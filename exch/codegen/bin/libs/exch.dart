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
      'Market_id_t = int32_t',
      'User_id_t = int64_t',
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
    ..customBlocks = [ fcbBeginNamespace ]
    ..includes = [
      'sstream'
    ]
    ..enums = [
      enum_('side')
      ..values = [ 'bid_side', 'ask_side' ],
      enum_('order_state')
      ..values = [ 'submitted', 'active', 'canceled', 'filled' ],
    ]
    ..classes = [

      class_('book_entry')
      ..struct = true
      ..streamable = true
      ..defaultCtor.useDefault = true
      ..memberCtors = [ memberCtor(['price', 'quantity']) ]
      ..members = [
        member('price')..type = 'Price_t'..cppAccess = public..noInit = true,
        member('quantity')..type = 'Quantity_t'..cppAccess = public..noInit = true,
      ],

      class_('market')
      ..struct = true
      ..memberCtors = [ memberCtor(['bid', 'ask']) ]
      ..streamable = true
      ..defaultCtor.useDefault = true
      ..members = [
        member('bid')..type = 'Book_entry'..cppAccess = public..noInit = true,
        member('ask')..type = 'Book_entry'..cppAccess = public..noInit = true,
      ],

      class_('order')
      ..immutable = true
      ..streamable = true
      ..customBlocks = [ clsPublic ]
      ..members = [
        member('order_id')..type = 'Order_id_t',
        member('timestamp')..type = 'Timestamp_t',
        member('side')..type = 'Side',
        member('quantity')..type = 'Quantity_t',
        member('price')..type = 'Price_t',
      ],
      class_('fill')
      ..immutable = true
      ..streamable = true
      ..members = [
        member('timestamp')..type = 'Timestamp_t',
        member('order')..type = 'Order_id_t',
        member('price')..type = 'Price_t',
        member('quantity')..type = 'Quantity_t',
      ]
    ],
    header('market')
    ..includes = [ 'exch/order_book.hpp' ]
    ..enums = [
      enum_('submit_result')
      ..values = [
        'submit_succeeded',
        'submit_invalid_market',
        'submit_invalid_order_details' ],

      enum_('cancel_result')
      ..values = [
        'cancel_succeeded',
        'cancel_invalid_market',
        'cancel_invalid_order'
      ],

      enum_('replace_result')
      ..values = [
        'replace_succeeded',
        'replace_invalid_market',
        'replace_invalid_order',
        'replace_invalid_order_details',
      ],
    ]
    ..forwardDecls = [
      forwardDecl('Market_exchange'),
      forwardDecl('Managed_order'),
    ]
    ..usings = [
      'Market_exchange_ptr = std::unique_ptr< Market_exchange >',
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
      class_('managed_order')
      ..customBlocks = [ clsPublic ]
      ..memberCtors = [ memberCtor([ 'order' ]) ]
      ..members = [
        member('order')..type = 'Order'..noInit = true..isConst = true
        ..cppAccess = public..byRef = true,
        member('order_state')..type = 'Order_state'..init = 'Submitted_e'..cppAccess = public,
        member('filled')..type = 'Quantity_t'..init = 0..cppAccess = public,
      ],
      class_('market_exchange')
      ..descr = 'Responsible for the exchange of a single market (e.g. one market id)'
      ..customBlocks = [ clsPublic ]
      ..dtor.abstract = true
      ..memberCtors = [ memberCtor([ 'market_config', 'market_id' ] )]
      ..members = [
        member('market_config')..type = 'Market_config'..noInit = true..byRef = true,
        member('market_id')..type = 'Market_id_t'..isConst = true,
        member('next_order_id')..init = 0,
      ],
      class_('market_exchange_factory')
      ..dtor.abstract = true
      ..customBlocks = [ clsPublic ]
      ..descr = 'Used to create market_exchange derivatives on demand',
    ],
    header('market_redis')
    ..includes = [
      'exch/market.hpp', 'redisclient/redisclient.h', 'boost/lexical_cast.hpp',
      'sstream',
    ]
    ..includeTest = true
    ..test.includes.addAll([ 'iostream' ])
    ..classes = [
      class_('market_exchange_redis')
      ..descr = 'Provides service for one market exchange, persisting books in redis'
      ..customBlocks = [ clsPublic, clsPrivate, clsPreDecl ]
      ..bases = [
        base('Market_exchange')
        ..init = 'Market_exchange(market_config, market_id)'
      ]
      ..memberCtors = [
        memberCtor([ 'redis_client' ])
        ..decls = [ 'Market_config const& market_config', 'Market_id_t market_id' ]
      ]
      ..members = [
        member('redis_client')..type = 'RedisClient'..refType = ref,
        member('market_id_str')..type = 'std::string'
        ..isConst = true
        ..ctorInit = 'boost::lexical_cast< std::string >(market_id)',
        member('orders_key')..type = 'std::string'
        ..isConst = true
        ..ctorInit = 'market_id_str_ + std::string(":ORDERS")',
      ],
      class_('market_exchange_redis_factory')
      ..customBlocks = [ clsPublic ]
      ..memberCtors = [ memberCtor([ 'redis_client' ]) ]
      ..members = [
        member('redis_client')..type = 'RedisClient'..refType = ref,
      ],
    ],
    header('market_mem')
    ..includes = [
      'exch/market.hpp',
    ]
    ..classes = [
      class_('market_exchange_mem')
      ..includeTest = true
      ..bases = [ base('Market_exchange') ]
      ..customBlocks = [ clsPublic ]
      ..members = [
        member('active_orders')..type = 'Managed_order_list_t',
        member('dead_orders')..type = 'Managed_order_list_t',
        member('net_volume')..type = 'Quantity_t',
      ]
    ],
    header('exchange')
    ..includes = [ 'exch/market.hpp', ]
    ..usings = [ 'Req_id_t = int64_t' ]
    ..enums = [
      enum_('create_market_result')
      ..values = [
        'create_market_succeeded',
        'create_market_failed', ],
    ]
    ..classes = [
      ////////////////////////////////////////////////////////////
      // Requests/Responses
      ////////////////////////////////////////////////////////////
      class_('create_market_req')
      ..immutable = true
      ..members = [
        member('req_id')..type = 'Req_id_t',
        member('market_id')..type = 'Market_id_t',
        member('name')..type = 'std::string',
        member('start_time')..type = 'Timestamp_t',
        member('end_time')..type = 'Timestamp_t',
        member('decimal_shift')..type = 'int',
        member('shifted_modulus')..type = 'int',
        member('max_price')..type = 'Price_t',
      ],
      class_('create_market_resp')
      ..immutable = true
      ..members = [
        member('req_id')..type = 'Req_id_t',
        member('market_id')..type = 'Market_id_t',
        member('result')..type = 'Create_market_result',
      ],

      class_('submit_req')
      ..immutable = true
      ..members = [
        member('req_id')..type = 'Req_id_t',
        member('market_id')..type = 'Market_id_t',
        member('user_id')..type = 'User_id_t',
        member('side')..type = 'Side',
        member('price')..type = 'Price_t',
        member('quantity')..type = 'Quantity_t',
      ],
      class_('submit_resp')
      ..immutable = true
      ..members = [
        member('req_id')..type = 'Req_id_t',
        member('market_id')..type = 'Market_id_t',
        member('user_id')..type = 'User_id_t',
        member('order_id')..type = 'Order_id_t',
        member('result')..type = 'Submit_result',
      ],

      class_('cancel_req')
      ..immutable = true
      ..members = [
        member('req_id')..type = 'Req_id_t',
        member('market_id')..type = 'Market_id_t',
        member('user_id')..type = 'User_id_t',
        member('order_id')..type = 'Order_id_t',
      ],
      class_('cancel_resp')
      ..immutable = true
      ..members = [
        member('req_id')..type = 'Req_id_t',
        member('market_id')..type = 'Market_id_t',
        member('user_id')..type = 'User_id_t',
        member('order_id')..type = 'Order_id_t',
        member('result')..type = 'Cancel_result',
      ],

      class_('replace_req')
      ..immutable = true
      ..members = [
        member('req_id')..type = 'Req_id_t',
        member('market_id')..type = 'Market_id_t',
        member('user_id')..type = 'User_id_t',
        member('existing_order_id')..type = 'Order_id_t',
        member('price')..type = 'Price_t',
        member('quantity')..type = 'Quantity_t',
      ],
      class_('replace_resp')
      ..immutable = true
      ..members = [
        member('req_id')..type = 'Req_id_t',
        member('market_id')..type = 'Market_id_t',
        member('user_id')..type = 'User_id_t',
        member('canceled_order_id')..type = 'Order_id_t',
        member('order_id')..type = 'Order_id_t',
        member('result')..type = 'Replace_result',
      ],

      ////////////////////////////////////////////////////////////
      // Events
      ////////////////////////////////////////////////////////////
      class_('market_created_evt')
      ..immutable = true
      ..members = [
        member('market_id')..type = 'Market_id_t',
      ],

      class_('top_of_book_evt')
      ..immutable = true
      ..members = [
        member('market_id')..type = 'Market_id_t',
      ],
      class_('book_update_evt')
      ..immutable = true
      ..members = [
        member('market_id')..type = 'Market_id_t',
        member('side')..type = 'Side',
        member('price')..type = 'Price_t',
        member('quantity')..type = 'Quantity_t',
        member('top_price')..type = 'Price_t',
      ],
      class_('trade_evt')
      ..immutable = true
      ..members = [
        member('market_id')..type = 'Market_id_t',
        member('side')..type = 'Side',
        member('quantity')..type = 'Quantity_t',
        member('price')..type = 'Price_t',
        member('net_volume')..type = 'Quantity_t',
      ],

      class_('market_publisher')
      ..dtor.abstract = true
      ..customBlocks = [ clsPublic ],

      class_('exchange')
      ..descr = '''
Manages multiple markets. Requests are forwarded to the appropriate
exchange for handling. Responses from the Market_exchange instances
are used to create and publish events using the Market_publisher. An
example publisher might serialize the responses to Json and post via
redis - but that is an implementation detail from the perspective of
this class.'''
      ..includeTest = true
      ..customBlocks = [ clsPublic, clsPrivate ]
      ..memberCtors = [
        memberCtor([ 'market_publisher', 'market_exchange_factory' ])..customLabel = 'from_args'
      ]
      ..usings = [
        'Market_exchange_naked_ptr = Market_exchange *',
        'Market_exchange_map_t = std::map< Market_id_t, Market_exchange_ptr >',
      ]
      ..members = [
        member('market_publisher')..type = 'Market_publisher'..refType = ref,
        member('market_exchange_factory')..type = 'Market_exchange_factory'..refType = ref,
        member('market_exchanges')..type = 'Market_exchange_map_t',
      ],
    ],
    header('redis_server')
    ..descr = '''
Uses redis pub/sub as means to accept requests destined to a
Market_exchange and publish responses destined for clients.'''
    ..classes = [

      class_('redis_listener')
      ..descr = '''
Subscribes to client requests on redis pub/sub channels''',
      class_('redis_publisher')
      ..descr = '''
Implements the Market_publisher interface using redis as pub/sub
middleware'''
      ..bases = [ base('Market_publisher') ]
      ..members = [],
      class_('redis_server')
      ..bases = [ base('Exchange_server') ]
      ..members = [
        member('exchange')..type = 'Exchange',
        member('listener')..type = 'RedisListener',
      ]
    ],
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
