#!/usr/bin/env dart
/// Creates a market, does sequence of other commands on that market

import 'dart:io';
import 'dart:math';
import 'package:args/args.dart';
import 'package:exch_client/exch_client.dart';
import 'package:logging/logging.dart';
import 'package:redis_client/redis_client.dart';

//! The parser for this script
ArgParser _parser;

//! The comment and usage associated with this script
void _usage() {
  print('''
Creates a market, does sequence of other commands on that market
''');
  print(_parser.getUsage());
}

//! Method to parse command line options.
//! The result is a map containing all options, including positional options
Map _parseArgs(List<String> args) {
  ArgResults argResults;
  Map result = { };
  List remaining = [];

  _parser = new ArgParser();
  try {
    /// Fill in expectations of the parser
    _parser.addFlag('help',
      help: '''
Display this help screen
''',
      abbr: 'h',
      defaultsTo: false
    );

    _parser.addOption('redis-host',
      help: '''
redis host used by pub/sub
''',
      defaultsTo: '127.0.0.1',
      allowMultiple: false,
      abbr: 'H',
      allowed: null
    );
    _parser.addOption('redis-port',
      help: '''
redis port used by pub/sub
''',
      defaultsTo: '6379',
      allowMultiple: false,
      abbr: 'P',
      allowed: null
    );
    _parser.addOption('req-id',
      help: '',
      defaultsTo: '1',
      allowMultiple: false,
      abbr: 'r',
      allowed: null
    );
    _parser.addOption('user-id',
      help: '',
      defaultsTo: '1',
      allowMultiple: false,
      abbr: 'u',
      allowed: null
    );
    _parser.addOption('market-id',
      help: '',
      defaultsTo: '1',
      allowMultiple: false,
      abbr: 'm',
      allowed: null
    );

    /// Parse the command line options (excluding the script)
    argResults = _parser.parse(args);
    if(argResults.wasParsed('help')) {
      _usage();
      exit(0);
    }
    result['redis-host'] = argResults['redis-host'];
    result['redis-port'] = argResults['redis-port'] != null?
      int.parse(argResults['redis-port']) : null;
    result['req-id'] = argResults['req-id'] != null?
      int.parse(argResults['req-id']) : null;
    result['user-id'] = argResults['user-id'] != null?
      int.parse(argResults['user-id']) : null;
    result['market-id'] = argResults['market-id'] != null?
      int.parse(argResults['market-id']) : null;
    result['help'] = argResults['help'];

    return { 'options': result, 'rest': remaining };

  } catch(e) {
    _usage();
    throw e;
  }
}

final _logger = new Logger('bootstrapScenario1');

main(List<String> args) {
  Logger.root.onRecord.listen((LogRecord r) =>
      print("${r.loggerName} [${r.level}]:\t${r.message}"));
  Logger.root.level = Level.INFO;
  Map argResults = _parseArgs(args);
  Map options = argResults['options'];
  List positionals = argResults['rest'];

  // custom <bootstrapScenario1 main>

  final host = options['redis-host'];
  final port = options['redis-port'];

  int userId = 42;
  int marketId = 1;
  int basePrice = 12530;

  final startTime = new DateTime.now();
  final endTime = startTime.add(new Duration(days: 2));
  //final random = new Random(42);
  final random = new Random();

  int maxAsk = 0;
  int minBid = 1<<30;

  someDelta() => random.nextInt(50);

  nextBid() {
    final result = basePrice - someDelta() + someDelta();
    minBid = min(minBid, result);
    return result;
  }

  nextAsk() {
    final result = basePrice + someDelta() - someDelta();
    maxAsk = max(maxAsk, result);
    return result;
  }

  nextQty() => 50 + random.nextInt(100);

  RedisClient
    .connect('$host:$port')
    .then((RedisClient redisClient) {
      final client = new ExchClient(redisClient);
      client.createMarket(createMarketReq(
            reqId, userId, "bootstrap_1", startTime, endTime, 2, 500));

      for(int i=1; i<=200; i++) {
        final big = (i%11==0);
        final qty = nextQty();

        if(someDelta()%2 == 0) {
          final px = nextAsk();
          client.submit(new SubmitReq(
                reqId, userId, marketId, Side.ASK_SIDE,
                big? minBid : px,
                big? qty*3 : qty));

        } else {
          final px = nextBid();
          client.submit(new SubmitReq(
                reqId, userId, marketId, Side.BID_SIDE,
                big? maxAsk : px,
                big? qty*3 : qty));
        }
      }

      redisClient.close();
    });


  // end <bootstrapScenario1 main>

}

// custom <bootstrapScenario1 global>
int _reqId = 0;
get reqId => _reqId++;
// end <bootstrapScenario1 global>
