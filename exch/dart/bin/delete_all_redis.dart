#!/usr/bin/env dart
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
null
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

    /// Parse the command line options (excluding the script)
    argResults = _parser.parse(args);
    if(argResults.wasParsed('help')) {
      _usage();
      exit(0);
    }
    result['redis-host'] = argResults['redis-host'];
    result['redis-port'] = argResults['redis-port'] != null?
      int.parse(argResults['redis-port']) : null;
    result['help'] = argResults['help'];

    return { 'options': result, 'rest': remaining };

  } catch(e) {
    _usage();
    throw e;
  }
}

final _logger = new Logger('deleteAllRedis');

main(List<String> args) {
  Logger.root.onRecord.listen((LogRecord r) =>
      print("${r.loggerName} [${r.level}]:\t${r.message}"));
  Logger.root.level = Level.INFO;
  Map argResults = _parseArgs(args);
  Map options = argResults['options'];
  List positionals = argResults['rest'];

  // custom <deleteAllRedis main>

  final host = options['redis-host'];
  final port = options['redis-port'];

  stdout.write('Are you sure you want to delete all redis keys? [y/n]: ');
  String input = stdin.readLineSync();

  RedisClient
    .connect('$host:$port')
    .then((RedisClient redisClient) {

      if(new RegExp(r'^(?:y|yes)$', caseSensitive:false).hasMatch(input)) {
        final keys = [ 'CMD', 'FILLS' ];
        print('Deleting redis keys $keys');
        return redisClient
          .mdel(keys)
          .then((int deleted) {
            print('Deleted $deleted keys');
            redisClient.close();
          });
      } else {
        print('Pheww - saved you there');
      }

    });

  // end <deleteAllRedis main>

}

// custom <deleteAllRedis global>
// end <deleteAllRedis global>
