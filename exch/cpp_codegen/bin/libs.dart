library libs;

import '../lib/installation.dart';
import 'libs/exch.dart' as exch;

addItems() {
  exch.addItems();
}

main() {
  addItems();
  print(installation);
  installation.generate();
}

// end <library libs>
