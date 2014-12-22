library bin.random_command_repository;

import 'package:ebisu_cpp/cpp.dart';
import '../../lib/installation.dart';

final repo = app('exch_server')
  ..descr = '''
Server supporting dynamic creation of markets exchanges
'''
  ..includes = [
    'exch/exchange.hpp',
    'exch/redis_support.hpp',
    'boost/asio/ip/address.hpp',
    'redisclient/redisclient.h',
  ]
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

addItems() => installation.addApp(repo);

main() {
  addItems();
  installation.generate();
}
