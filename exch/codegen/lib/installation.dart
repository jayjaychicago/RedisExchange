library installation;

import 'package:ebisu_cpp/cpp.dart';
import 'dart:io';
import 'package:id/id.dart';

String _getPath() {
  final script = Platform.script.path;
  return script.substring(0, script.indexOf('exch/codegen')) + 'exch';
}

final installation = new Installation(new Id('exch'))
  ..root = _getPath();
