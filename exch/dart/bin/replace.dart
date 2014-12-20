#!/usr/bin/env dart
/// Replace an order in a market

import 'dart:io';
import 'package:args/args.dart';
import 'package:exch_client/exch_client.dart';
import 'package:logging/logging.dart';
import 'package:redis_client/redis_client.dart';

//! The parser for this script
ArgParser _parser;

//! The comment and usage associated with this script
void _usage() {
  print('''
Replace an order in a market
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
    _parser.addOption('order-id',
      help: '',
      defaultsTo: '1',
      allowMultiple: false,
      abbr: 'o',
      allowed: null
    );
    _parser.addOption('price',
      help: '''
Price of an order
''',
      defaultsTo: '10000',
      allowMultiple: false,
      abbr: 'p',
      allowed: null
    );
    _parser.addOption('quantity',
      help: '''
Quantity of an order
''',
      defaultsTo: '100',
      allowMultiple: false,
      abbr: 'q',
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
    result['order-id'] = argResults['order-id'] != null?
      int.parse(argResults['order-id']) : null;
    result['price'] = argResults['price'] != null?
      int.parse(argResults['price']) : null;
    result['quantity'] = argResults['quantity'] != null?
      int.parse(argResults['quantity']) : null;
    result['help'] = argResults['help'];

    return { 'options': result, 'rest': remaining };

  } catch(e) {
    _usage();
    throw e;
  }
}

final _logger = new Logger('replace');

main(List<String> args) {
  Logger.root.onRecord.listen((LogRecord r) =>
      print("${r.loggerName} [${r.level}]:\t${r.message}"));
  Logger.root.level = Level.INFO;
  Map argResults = _parseArgs(args);
  Map options = argResults['options'];
  List positionals = argResults['rest'];

  // custom <replace main>

  final host = options['redis-host'];
  final port = options['redis-port'];
  RedisClient
    .connect('$host:$port')
    .then((RedisClient redisClient) {
      final client = new ExchClient(redisClient);
      final req= new ReplaceReq(
        options['req-id'], options['user-id'],
        options['market-id'], options['order-id'],
        options['price'], options['quantity']);
      client.replace(req);
      redisClient.close();
    });

  // end <replace main>

}

// custom <replace global>
// end <replace global>

