import "dart:io";
import "package:path/path.dart" as path;
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

get commonArgs => [ redisHostArg, redisPort, reqIdArg, userIdArg ];
get priceArg => scriptArg('price')..defaultsTo = 10000..abbr = 'p';
get quantityArg => scriptArg('quantity')..defaultsTo = 100..abbr = 'q';

get commonImports => [
  'package:redis_client/redis_client.dart',
  'package:exch_client/exch_client.dart',
];

void main() {
  Logger.root.onRecord.listen((LogRecord r) =>
      print("${r.loggerName} [${r.level}]:\t${r.message}"));
  String here = path.absolute(Platform.script.path);
  _topDir = path.join(path.dirname(path.dirname(here)), 'dart');

  System exchClient = system('exch_client')
    ..includeHop = true
    ..rootPath = '$_topDir'
    ..doc = 'Client exchange related services'
    ..testLibraries = [
    ]
    ..scripts = [
      script('create')
      ..imports = commonImports
      ..args = (commonArgs
          ..addAll([
            scriptArg('market_name')..defaultsTo = 'mkt-1'..abbr = 'm',
            scriptArg('decimal_shift')..defaultsTo = 2..abbr = 's',
            scriptArg('tick_size')..defaultsTo = 1..abbr = 't',
          ]))
      ..doc = 'Create a market',
      script('submit')
      ..imports = commonImports
      ..args = (commonArgs
          ..addAll([
            marketIdArg, priceArg, quantityArg,
          ]))
      ..doc = 'Submit an order to a market',
      script('cancel')
      ..imports = commonImports
      ..args = (commonArgs
          ..addAll([
            marketIdArg, orderIdArg,
          ]))
      ..doc = 'Cancel an order from a market',
      script('replace')
      ..imports = commonImports
      ..args = (commonArgs
          ..addAll([
            marketIdArg, orderIdArg,
            priceArg, quantityArg,
          ]))
      ..doc = 'Replace an order in a market',
    ]
    ..libraries = [
      library('exch_client')
      ..imports = [
      ]
      ..parts = [
        part('requests')
        ..enums = [
          enum_('side')
          ..values = [ id('bid_side'), id('ask_side') ]
        ]
        ..classes = [
          class_('create_market_req')
          ..jsonKeyFormat = snake
          ..immutable = true
          ..members = [
            member('req_id')..type = 'int',
            member('user_id')..type = 'int',
            member('name')..type = 'int',
            member('start_time'),
            member('end_time'),
            member('decimal_shift')..classInit = 2,
            member('tick_size')..classInit = 1,
          ],
          class_('submit_req')
          ..jsonKeyFormat = snake
          ..immutable = true
          ..members = [
            member('req_id')..type = 'int',
            member('market_id')..type = 'int',
            member('user_id')..type = 'int',
            member('side')..type = 'Side',
            member('price')..type = 'int',
            member('quantity')..type = 'int',
          ],
          class_('cancel_req')
          ..jsonKeyFormat = snake
          ..immutable = true
          ..members = [
            member('req_id')..type = 'int',
            member('market_id')..type = 'int',
            member('user_id')..type = 'int',
            member('order_id')..type = 'int',
          ],
          class_('replace_req')
          ..jsonKeyFormat = snake
          ..immutable = true
          ..members = [
            member('req_id')..type = 'int',
            member('market_id')..type = 'int',
            member('user_id')..type = 'int',
            member('existing_order_id')..type = 'int',
            member('price')..type = 'int',
            member('quantity')..type = 'int',
          ],
        ],
        part('events'),
        part('exch_client')
        ..classes = [
          class_('exch_client')
          //..jsonKeyFormat = snake
          ..members = [
          ]
        ],
      ]
    ];

  exchClient.generate();
}
