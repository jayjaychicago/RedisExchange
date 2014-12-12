library bin.exch;

import 'package:ebisu_cpp/cpp.dart';
import 'package:logging/logging.dart';
import '../../lib/installation.dart';

final random_command = lib('random_command')
  ..namespace = namespace([ 'random_command' ])
  ..headers = [
    header('random_command')
    ..isApiHeader = true
    ..includes = [
      'exch/requests.hpp', 'map', 'vector', 'utility', 'iosfwd', 'random'
    ]
    ..customBlocks = [ fcbEndNamespace ]
    ..enums = [
    ]
    ..usings = [
      'Price_t = exch::Price_t',
      'Price_range_t = std::pair< Price_t, Price_t >',
    ]
    ..classes = [
      class_('market_commands_spec')
      ..immutable = true
      ..streamable = true
      ..members = [
        member('seed')..type = 'unsigned int',
        member('center_price')..type = 'Price_t',
        member('valid_submits')..type = 'int',
        member('invalid_submits')..type = 'int',
        member('valid_cancels')..type = 'int',
        member('invalid_cancels')..type = 'int',
        member('valid_replaces')..type = 'int',
        member('invalid_replaces')..type = 'int',
      ],
      class_('command_generator')
      ..memberCtors = [ memberCtor(['spec']) ]
      ..customBlocks = [ clsPublic ]
      ..members = [
        member('spec')..type = 'Market_commands_spec'..byRef = true..noInit = true,
        member('engine')..type = 'std::mt19937'
        ..mutable = true
        ..ctorInit = 'spec_.seed()',
        member('min_price')..type = 'Price_t'
        ..initText = 'int(0.5 * spec_.center_price())'
        ..isConst = true,
        member('max_price')..type = 'Price_t'
        ..initText = 'int(2.0 * spec_.center_price())'
        ..isConst = true,
        member('dist')..type = 'std::uniform_int_distribution< Price_t >'
        ..mutable = true
        ..initText = 'min_price_, max_price_',
      ]
    ],
  ];

addItems() => installation.addLibs([random_command]);

main() {
  Logger.root
    ..onRecord.listen((LogRecord r) =>
        print("${r.loggerName} [${r.level}]:\t${r.message}"))
    ..level = Level.WARNING;
  addItems();
  installation.generate();
}
