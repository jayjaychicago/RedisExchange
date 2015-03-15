import "dart:io";
import "package:path/path.dart" as path;
import "package:ebisu/ebisu.dart";
import "package:ebisu/ebisu_dart_meta.dart";
import "package:logging/logging.dart";
import "package:id/id.dart";

String _topDir;

get redisHostArg => scriptArg('redis_host')
  ..doc = 'redis host used by pub/sub'
  ..abbr = 'H'
  ..defaultsTo = "'127.0.0.1'";

get redisPort => scriptArg('redis_port')
  ..doc = 'redis port used by pub/sub'
  ..abbr = 'P'
  ..defaultsTo = 6379;

get reqIdArg => scriptArg('req_id')..type = ArgType.INT..abbr = 'r'..defaultsTo = 1;
get userIdArg => scriptArg('user_id')..type = ArgType.INT..abbr = 'u'..defaultsTo = 1;
get marketIdArg => scriptArg('market_id')..type = ArgType.INT..abbr = 'm'..defaultsTo = 1;
get orderIdArg => scriptArg('order_id')..type = ArgType.INT..abbr = 'o'..defaultsTo = 1;
get sideArg => scriptArg('is_ask')
  ..doc = 'Is this a bid side (default) or ask side'
  ..isFlag = true..abbr = 'a';

get commonArgs => [ redisHostArg, redisPort, userIdArg ];
get priceArg => scriptArg('price')..doc = 'Price of an order'..defaultsTo = 10000..abbr = 'p';
get quantityArg => scriptArg('quantity')..doc = 'Quantity of an order'..defaultsTo = 100..abbr = 'q';

get commonImports => [
  'package:redis_client/redis_client.dart',
  'package:exch_client/exch_client.dart',
];

void main() {
  Logger.root.onRecord.listen((LogRecord r) =>
      print("${r.loggerName} [${r.level}]:\t${r.message}"));
  String here = path.absolute(Platform.script.path);
  _topDir = path.join(path.dirname(path.dirname(here)), 'dart');

  useDartFormatter = true;

  System exchClient = system('exch_client')
    ..includeHop = true
    ..rootPath = '$_topDir'
    ..doc = 'Client exchange related services'
    ..testLibraries = [
    ]
    ..scripts = [

      script('delete_all_redis')
      ..imports = commonImports
      ..args = [redisHostArg, redisPort],

      script('market_details')
      ..isAsync = true
      ..imports = commonImports
      ..args = [
        redisHostArg, redisPort, reqIdArg, marketIdArg,
        scriptArg('user_id')..type = ArgType.INT..abbr = 'u'..defaultsTo = 0,
        scriptArg('start_time')
        ..doc = 'Start time for filtering (inclusive): '
        ..abbr = 's'
        ..defaultsTo = '2000-01-01 00:00:00',
        scriptArg('end_time')
        ..doc = 'End time for filtering (exclusive): '
        ..abbr = 'e'
        ..defaultsTo = '2100-01-01 00:00:00',
        scriptArg('include_active')..doc = 'Include active orders in response'
        ..isFlag = true..defaultsTo = true,
        scriptArg('include_dead')..doc = 'Include dead orders in response'
        ..isFlag = true..defaultsTo = true,
        scriptArg('include_fills')..doc = 'Include fills in response'
        ..isFlag = true..defaultsTo = true,
      ],

      script('create')
      ..isAsync = true
      ..imports = commonImports
      ..args = (commonArgs
          ..addAll([
            scriptArg('market_name')
            ..doc = 'Name of market to be created'
            ..defaultsTo = 'mkt-1'..abbr = 'm',
            scriptArg('start_time')
            ..doc = 'Time market begins - sample format: '
            ..defaultsTo = '2015-01-03 13:30:00',
            scriptArg('end_time')
            ..doc = 'Time market end - sample format: '
            ..defaultsTo = '2015-01-03 14:30:00',
            scriptArg('decimal_shift')
            ..doc = 'Number of decimal digits to shift for display purposes'
            ..defaultsTo = 2..abbr = 's',
            scriptArg('tick_size')
            ..doc = '''
Increment for price = e.g. decimal-shift of 2 and tick-size of 1 means display
is dollars and cents, with prices occuring at every penny. decimal-shift of 2
and tick-size of 5 means display is in dollars and cents and prices vary by
dimes (i.e. 100.07 is not valid but 100.05 is)
'''
            ..defaultsTo = 1..abbr = 't',
          ]))
      ..doc = 'Create a market',
      script('submit')
      ..isAsync = true
      ..imports = commonImports
      ..args = (commonArgs
          ..addAll([
            marketIdArg, sideArg, priceArg, quantityArg,
          ]))
      ..doc = 'Submit an order to a market',
      script('cancel')
      ..isAsync = true
      ..imports = commonImports
      ..args = (commonArgs
          ..addAll([
            marketIdArg, orderIdArg,
          ]))
      ..doc = 'Cancel an order from a market',
      script('log')
      ..isAsync = true
      ..imports = commonImports
      ..args = (commonArgs
          ..addAll([
            marketIdArg,
          ]))
      ..doc = 'Log current state of a market',
      script('replace')
      ..isAsync = true
      ..imports = commonImports
      ..args = (commonArgs
          ..addAll([
            marketIdArg, orderIdArg,
            priceArg, quantityArg,
          ]))
      ..doc = 'Replace an order in a market',
      script('bootstrap_scenario_1')
      ..isAsync = true
      ..doc = 'Creates a market, does sequence of other commands on that market'
      ..imports = (commonImports..addAll([ 'math', 'async' ]))
      ..args = (commonArgs..addAll(
              [
                marketIdArg,
                scriptArg('log_at_end')
                ..doc = 'If set will log market details after work complete'
                ..isFlag = true..defaultsTo = true,
              ])),
    ]
    ..libraries = [
      library('exch_client')
      ..imports = [
        'convert',
        'package:redis_client/redis_client.dart',
        'package:logging/logging.dart',
        'collection',
        'async',
        'io',
      ]
      ..includeLogger = true
      ..parts = [
        part('requests')
        ..enums = [
          enum_('side')
          ..jsonSupport = true
          ..values = [ id('bid_side'), id('ask_side') ],
          enum_('log_type')
          ..jsonSupport = true
          ..values = [ id('log_book') ],
        ]
        ..classes = [
          class_('request_completer')
          ..immutable = true
          ..members = [
            member('request')..type = 'Request',
            member('completer')..type = 'Completer',
          ],
          class_('request')
          ..isAbstract = true,
          class_('create_market_req')
          ..jsonKeyFormat = snake
          ..immutable = true
          ..implement = ['Request']
          ..members = [
            member('req_id')..type = 'int',
            member('user_id')..type = 'int',
            member('name'),
            member('start_time')
            ..doc = 'seconds since epoch indicating start time'
            ..type = 'int',
            member('end_time')
            ..doc = 'seconds since epoch indicating end time'
            ..type = 'int',
            member('decimal_shift')..classInit = 2,
            member('tick_size')..classInit = 1,
          ],
          class_('submit_req')
          ..jsonKeyFormat = snake
          ..implement = ['Request']
          ..immutable = true
          ..members = [
            member('req_id')..type = 'int',
            member('user_id')..type = 'int',
            member('market_id')..type = 'int',
            member('side')..type = 'Side',
            member('price')..type = 'int',
            member('quantity')..type = 'int',
          ],
          class_('cancel_req')
          ..jsonKeyFormat = snake
          ..immutable = true
          ..implement = ['Request']
          ..members = [
            member('req_id')..type = 'int',
            member('user_id')..type = 'int',
            member('market_id')..type = 'int',
            member('order_id')..type = 'int',
          ],
          class_('replace_req')
          ..jsonKeyFormat = snake
          ..immutable = true
          ..implement = ['Request']
          ..members = [
            member('req_id')..type = 'int',
            member('user_id')..type = 'int',
            member('market_id')..type = 'int',
            member('order_id')..type = 'int',
            member('price')..type = 'int',
            member('quantity')..type = 'int',
          ],
          class_('market_details_req')
          ..jsonKeyFormat = snake
          ..implement = ['Request']
          ..immutable = true
          ..members = [
            member('req_id')..type = 'int',
            member('market_id')..type = 'int',
            member('user_id')..type = 'int',
            member('start_time')
            ..doc = 'seconds since epoch indicating start time'
            ..type = 'int',
            member('end_time')
            ..doc = 'seconds since epoch indicating end time'
            ..type = 'int',
            member('include_active')..type = 'bool',
            member('include_dead')..type = 'bool',
            member('include_fills')..type = 'bool',
          ],
          class_('log_req')
          ..jsonKeyFormat = snake
          ..immutable = true
          ..members = [
            member('log_type')..type = 'LogType',
            member('market_id')..type = 'int',
          ],
        ],
        part('events'),
        part('exch_client')
        ..classes = [
          class_('exch_client')
          ..members = [
            member('host'),
            member('port')..type = 'int',
            member('publish_client')..type = 'RedisClient'..access = IA,
            member('listener_client')..type = 'RedisClient'..access = IA,
            member('next_req_id')..classInit = 1..access = IA,
            member('requests')..type = 'Queue<RequestCompleter>'
            ..access = IA..classInit = 'new Queue()',
          ]
        ],
      ]
    ];

  exchClient.generate();
}
