#!/usr/bin/env dart
/// Creates a market, does sequence of other commands on that market

import 'dart:async';
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
    _parser.addFlag('log-at-end',
      help: '''
If set will log market details after work complete
''',
      abbr: null,
      defaultsTo: true
    );
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
    result['user-id'] = argResults['user-id'] != null?
      int.parse(argResults['user-id']) : null;
    result['market-id'] = argResults['market-id'] != null?
      int.parse(argResults['market-id']) : null;
    result['log-at-end'] = argResults['log-at-end'];
    result['help'] = argResults['help'];

    return { 'options': result, 'rest': remaining };

  } catch(e) {
    _usage();
    throw e;
  }
}

final _logger = new Logger('bootstrapScenario1');

main(List<String> args) async {
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
  //  final random = new Random();
  final random = new Random(42);

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
  final client = (await ExchClient.makeClient(host, port));

  List<Future> futures = [];

  futures.add(client.createMarket(
          userId, "bootstrap_1", startTime, endTime,
          decimalShift:2, tickSize:500));


  for(int i=1; i<=800; i++) {
    final big = (i%11==0);
    final qty = nextQty();

    if(i == 100) {
      futures.add(client.cancel(userId, marketId, 81));
      futures.add(client.cancel(userId, marketId, 93));
      futures.add(client.cancel(userId, marketId, 89));
      futures.add(client.cancel(userId, marketId, 31));
    }

    if(someDelta()%2 == 0) {
      final px = nextAsk();
      futures.add(
          client.submit(userId, marketId, true,
              big? minBid : px,
              big? qty*3 : qty));
    } else {
      final px = nextBid();
      futures.add(
          client.submit(userId, marketId, false,
              big? maxAsk : px,
              big? qty*3 : qty));
    }
  }

  futures.add(client.logBook(marketId));

  Future
  .wait(futures)
  .then((List futures) async {
    for(var response in futures) {
      if(_logger.level <= Level.FINEST) {
        _logger.finest('Got $response');
      }
    }
    if(options['log-at-end']) {
      String details = await client.marketDetails(marketId, true, true, true);
      print(details);
    }
    client.close();
  });

  // end <bootstrapScenario1 main>

}

// custom <bootstrapScenario1 global>
// end <bootstrapScenario1 global>
