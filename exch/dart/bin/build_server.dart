#!/usr/bin/env dart
import 'dart:async';
import 'dart:io';
import 'package:args/args.dart';
import 'package:logging/logging.dart';

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
    _parser.addFlag('release',
      help: '''
If set builds release, otherwise builds debug
''',
      abbr: 'r',
      defaultsTo: false
    );
    _parser.addFlag('help',
      help: '''
Display this help screen
''',
      abbr: 'h',
      defaultsTo: false
    );

    /// Parse the command line options (excluding the script)
    argResults = _parser.parse(args);
    if(argResults.wasParsed('help')) {
      _usage();
      exit(0);
    }
    result['release'] = argResults['release'];
    result['help'] = argResults['help'];

    return { 'options': result, 'rest': remaining };

  } catch(e) {
    _usage();
    throw e;
  }
}

final _logger = new Logger('buildServer');

main(List<String> args) {
  Logger.root.onRecord.listen((LogRecord r) =>
      print("${r.loggerName} [${r.level}]:\t${r.message}"));
  Logger.root.level = Level.INFO;
  Map argResults = _parseArgs(args);
  Map options = argResults['options'];
  List positionals = argResults['rest'];

  // custom <buildServer main>

  final isRelease = options['release'];
  final cppPath = '../../cpp';
  cppRelPath(String p) => '${cppPath}/$p';

  //////////////////////////////////////////////////////////////////////
  // Following must be updated to point to correct place
  //////////////////////////////////////////////////////////////////////
  final boostLibPath = '/usr/local/Cellar/boost/1.56.0/lib';
  boostLib(String lib) => '$boostLibPath/libboost_$lib-mt.a';
  final formatLib = '/Users/dbdavidson/install/lib/libformat.a';

  final includes = [
    '/Users/dbdavidson/dev/open_source/fcs/cpp',
    '/Users/dbdavidson/dev/open_source/redisclient/src',
    '/Users/dbdavidson/dev/open_source/cereal/include',
    '/Users/dbdavidson/install/include',                 // cppformat
    '/usr/local/Cellar/boost/1.56.0/include',
    cppPath,
  ];


  //////////////////////////////////////////////////////////////////////
  // Now build
  //////////////////////////////////////////////////////////////////////
  final buildPath = isRelease? '../../build/release' : '../../build/debug';
  new Directory(buildPath).createSync(recursive:true);

  final srcs = [
    cppRelPath('app/exch_server/exch_server.cpp')
  ].join(' ');

  final boostLibs = [
    boostLib('system'),
    boostLib('thread'),
    boostLib('program_options'),
  ].join(' ');

  final includeArgs = '-I'+includes.join(' -I');

  final commands = options['release']?
    [
      '''
g++ -ftemplate-depth-128 -O3 -finline-functions -Wno-inline -Wall -fPIC -dynamic -Wno-long-double -no-cpp-precomp -std=c++11 -DNDEBUG ${includeArgs} -c -o $buildPath/exch_server.o $srcs''',
      '''
g++ -o $buildPath/exch_server $buildPath/exch_server.o $boostLibs $formatLib -lpthread'''
    ] :
    [
      '''
g++  -ftemplate-depth-128 -O0 -fno-inline -Wall -g -fPIC -dynamic -Wno-long-double -no-cpp-precomp -std=c++11 ${includeArgs} -c -o $buildPath/exch_server.o $srcs''',
      '''
g++ -o $buildPath/exch_server $buildPath/exch_server.o $boostLibs $formatLib -lpthread'''
    ];

  for(var cmd in commands) {
    final parts = cmd.split(' ');
    print("Running $cmd");
    final result = Process
      .runSync(parts[0], parts.sublist(1),
          workingDirectory:Directory.current.path,
          runInShell:true);
    print('''
Done with $cmd
================= STDOUT ==================
${result.stdout}
================= STDERR ==================
${result.stderr}
''');
  }

  // end <buildServer main>

}

// custom <buildServer global>
// end <buildServer global>

