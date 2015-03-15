#!/usr/bin/env dart
/// Create a market

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
Create a market
''');
  print(_parser.getUsage());
}

//! Method to parse command line options.
//! The result is a map containing all options, including positional options
Map _parseArgs(List<String> args) {
  ArgResults argResults;
  Map result = {};
  List remaining = [];

  _parser = new ArgParser();
  try {
    /// Fill in expectations of the parser
    _parser.addFlag('help', help: '''
Display this help screen
''', abbr: 'h', defaultsTo: false);

    _parser.addOption('redis-host', help: '''
redis host used by pub/sub
''', defaultsTo: '127.0.0.1', allowMultiple: false, abbr: 'H', allowed: null);
    _parser.addOption('redis-port', help: '''
redis port used by pub/sub
''', defaultsTo: '6379', allowMultiple: false, abbr: 'P', allowed: null);
    _parser.addOption('user-id',
        help: '',
        defaultsTo: '1',
        allowMultiple: false,
        abbr: 'u',
        allowed: null);
    _parser.addOption('market-name', help: '''
Name of market to be created
''', defaultsTo: 'mkt-1', allowMultiple: false, abbr: 'm', allowed: null);
    _parser.addOption('start-time', help: '''
Time market begins - sample format: 
''',
        defaultsTo: '2015-01-03 13:30:00',
        allowMultiple: false,
        abbr: null,
        allowed: null);
    _parser.addOption('end-time', help: '''
Time market end - sample format: 
''',
        defaultsTo: '2015-01-03 14:30:00',
        allowMultiple: false,
        abbr: null,
        allowed: null);
    _parser.addOption('decimal-shift', help: '''
Number of decimal digits to shift for display purposes
''', defaultsTo: '2', allowMultiple: false, abbr: 's', allowed: null);
    _parser.addOption('tick-size', help: '''
Increment for price = e.g. decimal-shift of 2 and tick-size of 1 means display
is dollars and cents, with prices occuring at every penny. decimal-shift of 2
and tick-size of 5 means display is in dollars and cents and prices vary by
dimes (i.e. 100.07 is not valid but 100.05 is)

''', defaultsTo: '1', allowMultiple: false, abbr: 't', allowed: null);

    /// Parse the command line options (excluding the script)
    argResults = _parser.parse(args);
    if (argResults.wasParsed('help')) {
      _usage();
      exit(0);
    }
    result['redis-host'] = argResults['redis-host'];
    result['redis-port'] = argResults['redis-port'] != null
        ? int.parse(argResults['redis-port'])
        : null;
    result['user-id'] =
        argResults['user-id'] != null ? int.parse(argResults['user-id']) : null;
    result['market-name'] = argResults['market-name'];
    result['start-time'] = argResults['start-time'];
    result['end-time'] = argResults['end-time'];
    result['decimal-shift'] = argResults['decimal-shift'] != null
        ? int.parse(argResults['decimal-shift'])
        : null;
    result['tick-size'] = argResults['tick-size'] != null
        ? int.parse(argResults['tick-size'])
        : null;
    result['help'] = argResults['help'];

    return {'options': result, 'rest': remaining};
  } catch (e) {
    _usage();
    throw e;
  }
}

final _logger = new Logger('create');

main(List<String> args) async {
  Logger.root.onRecord.listen(
      (LogRecord r) => print("${r.loggerName} [${r.level}]:\t${r.message}"));
  Logger.root.level = Level.INFO;
  Map argResults = _parseArgs(args);
  Map options = argResults['options'];
  List positionals = argResults['rest'];

  // custom <create main>

  final host = options['redis-host'];
  final port = options['redis-port'];
  final startTime = DateTime.parse(options['start-time']);
  final endTime = DateTime.parse(options['end-time']);

  final client = (await ExchClient.makeClient(host, port));
  client
      .createMarket(
          options['user-id'], options['market-name'], startTime, endTime,
          decimalShift: options['decimal-shift'],
          tickSize: options['tick-size'])
      .then((String response) {
    print('Response:\n$response');
    client.close();
  });

  // end <create main>

}

// custom <create global>
// end <create global>
