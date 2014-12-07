library bin.exch;

import 'package:ebisu_cpp/cpp.dart';
import 'package:logging/logging.dart';
import '../../lib/installation.dart';

final exch = lib('exch')
  ..namespace = namespace([ 'exch' ])
  ..headers = [
    header('exch')
    ..isApiHeader = true
    ..includes = [ 'cstdint', 'vector', 'utility',
      'fcs/timestamp/timestamp.hpp' ]
    ..forwardDecls = [
      forwardDecl('Order'),
    ]
    ..usings = [
      'Price_t = int32_t',
      'Quantity_t = int32_t',
      'Order_id_t = int64_t',
      'Order_id_list_t = std::vector< Order_id_t >',
      'Order_list_t = std::vector< Order >',
      'Timestamp_t = fcs::timestamp::Timestamp_t',
      'Order_update_t = std::pair< Order_id_t, Order >',
      'Order_update_list_t = std::vector< Order_update_t >',
    ],
    header('order_book')
    ..includes = [
    ]
    ..enums = [
      enum_('order_type')
      ..values = [ 'buy','sell', ],
      enum_('order_state')
      ..values = [ 'submitted', 'active', 'canceled', 'filled' ],
    ]
    ..classes = [

      class_('book_entry')
      ..struct = true
      ..defaultCtor.useDefault = true
      ..memberCtors = [ memberCtor(['price', 'quantity']) ]
      ..members = [
        member('price')..type = 'Price_t'..cppAccess = public..noInit = true,
        member('quantity')..type = 'Quantity_t'..cppAccess = public..noInit = true,
      ],

      class_('market')
      ..struct = true
      ..memberCtors = [ memberCtor(['bid', 'ask']) ]
      ..defaultCtor.useDefault = true
      ..members = [
        member('bid')..type = 'Book_entry'..cppAccess = public..noInit = true,
        member('ask')..type = 'Book_entry'..cppAccess = public..noInit = true,
      ],

      class_('order')
      ..immutable = true
      ..members = [
        member('timestamp')..type = 'Timestamp_t',
        member('quantity')..type = 'Quantity_t',
        member('price')..type = 'Price_t',
      ],
      class_('managed_order')
      ..customBlocks = [ clsPublic ]
      ..memberCtors = [ memberCtor([ 'order_id', 'order' ]) ]
      ..members = [
        member('order_id')..type = 'Order_id_t'..isConst = true..cppAccess = public,
        member('order')..type = 'Order'..noInit = true..isConst = true
        ..cppAccess = public..byRef = true,
        member('order_state')..type = 'Order_state'..init = 'Submitted_e'..cppAccess = public,
        member('filled')..type = 'Quantity_t'..init = 0..cppAccess = public,
      ],
      class_('fill')
      ..immutable = true
      ..members = [
        member('timestamp')..type = 'Timestamp_t',
        member('order')..type = 'Order_id_t',
        member('price')..type = 'Price_t',
        member('quantity')..type = 'Quantity_t',
      ]
    ],
    header('market')
    ..includes = [ 'exch/order_book.hpp' ]
    ..usings = [
      'Managed_order_list_t = std::vector< Managed_order >',
    ]
    ..classes = [
      class_('market_config')
      ..immutable = true
      ..streamable = true
      ..members = [
        member('name')..type = 'std::string',
        member('start_time')..type = 'Timestamp_t',
        member('end_time')..type = 'Timestamp_t',
        member('decimal_shift')..type = 'int',
        member('shifted_modulus')..type = 'int',
        member('max_price')..type = 'Price_t',
      ],
      class_('market_exchange')
      ..customBlocks = [ clsPublic ]
      ..dtor.abstract = true
      ..memberCtors = [ memberCtor(['market_config']) ]
      ..members = [
        member('market_config')..type = 'Market_config'..noInit = true,
        member('top')..type = 'Market'..cppAccess = protected..access = ro,
      ]

      ,
    ],
    header('market_redis')
    ..classes = [
      class_('market_exchange_redis')
      ..bases = [ base('Market_exchange') ]
    ],
    header('market_mem')
    ..includes = [
      'exch/market.hpp',
      'exch/publish.hpp',
    ]
    ..classes = [
      class_('market_exchange_mem')
      ..includeTest = true
      ..bases = [ base('Market_exchange') ]
      ..customBlocks = [ clsPublic ]
      ..memberCtors = [ memberCtor(['market_publisher']) ]
      ..members = [
        member('market_publisher')..type = 'Market_publisher'..refType = ref,
        member('active_orders')..type = 'Managed_order_list_t',
        member('dead_orders')..type = 'Managed_order_list_t',
        member('net_volume')..type = 'Quantity_t',
      ]
    ],
    header('publish')
    ..classes = [
      class_('market_publisher')
      ..dtor.abstract = true
      ..customBlocks = [ clsPublic ],
      class_('redis_publisher')
      ..bases = [ base('Market_publisher') ]
      ..members = []
    ]
  ];

addItems() => installation.addLibs([exch]);

main() {
  Logger.root
    ..onRecord.listen((LogRecord r) =>
        print("${r.loggerName} [${r.level}]:\t${r.message}"))
    ..level = Level.WARNING;
  addItems();
  installation.generate();
}
