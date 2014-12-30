import 'libs.dart' as libs;
import '../lib/installation.dart';
import 'apps/exch_server.dart' as exchange_server;
import 'package:ebisu_cpp/cpp.dart';
import 'package:logging/logging.dart';


main() {
  Logger.root
    ..onRecord.listen((LogRecord r) =>
        print("${r.loggerName} [${r.level}]:\t${r.message}"))
    ..level = Level.WARNING;
  libs.addItems();
  exchange_server.addItems();
  installation
    ..builders = [ cmakeInstallationBuilder() ]
    ..generate(true);
}
