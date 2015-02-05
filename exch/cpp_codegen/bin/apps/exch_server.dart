library bin.random_command_repository;

import 'package:ebisu_cpp/cpp.dart';
import '../../lib/installation.dart';

final server = app('exch_server')
  ..descr = '''
Server supporting dynamic creation of markets exchanges
'''
  ..includes = [
    'exch/exchange.hpp',
    'exch/redis_support.hpp',
    'boost/asio/ip/address.hpp',
    'hiredis/adapters/libuv.h',
  ]
  ..requiredLibs = [ 'format', 'boost_system', 'boost_thread' ]
  ..namespace = namespace([ 'exch_server' ])
  ..args = [

    arg('redis_address')
    ..descr = 'Address of redis - used for pub/sub and some persistence'
    ..shortName = 'a'
    ..defaultValue = '127.0.0.1',

    arg('redis_port')
    ..descr = 'Port for redis'
    ..shortName = 'p'
    ..defaultValue = 6379,

  ];

addItems() => installation.addApp(server);

main() {
  addItems();
  installation.generate();
}
