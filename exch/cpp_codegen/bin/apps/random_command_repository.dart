library bin.random_command_repository;

import 'package:ebisu_cpp/cpp.dart';
import '../../lib/installation.dart';

final repo = app('random_command_repository')
  ..descr = '''
Uses the random_command library to create a flat file with sequences of random
commands to push through the exchange for one market.
'''
  ..includes = [ 'random_command/random_command.hpp' ]
  ..args = [

    arg('seed')
    ..descr = 'Random seed to allow determinism'
    ..shortName = 's'
    ..defaultValue = 42,

    arg('center_price')
    ..descr = 'Center price for generated random prices'
    ..shortName = 'p'
    ..defaultValue = 100,

    arg('valid_submits')
    ..descr = 'Number of valid submits to generate'
    ..defaultValue = 1000,
    arg('invalid_submits')
    ..descr = 'Number of invalid submits to generate'
    ..defaultValue = 10,

    arg('valid_cancels')
    ..descr = 'Number of valid cancels to generate'
    ..defaultValue = 50,
    arg('invalid_cancels')
    ..descr = 'Number of invalid cancels to generate'
    ..defaultValue = 10,

    arg('valid_replaces')
    ..descr = 'Number of valid replaces to generate'
    ..defaultValue = 50,
    arg('invalid_replaces')
    ..descr = 'Number of invalid replaces to generate'
    ..defaultValue = 10,

  ];

addItems() => installation.addApp(repo);

main() {
  addItems();
  installation.generate();
}
