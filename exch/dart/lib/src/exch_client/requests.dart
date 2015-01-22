part of exch_client.exch_client;

class Side implements Comparable<Side> {
  static const BID_SIDE = const Side._(0);
  static const ASK_SIDE = const Side._(1);

  static get values => [
    BID_SIDE,
    ASK_SIDE
  ];

  final int value;

  int get hashCode => value;

  const Side._(this.value);

  copy() => this;

  int compareTo(Side other) => value.compareTo(other.value);

  String toString() {
    switch(this) {
      case BID_SIDE: return "BidSide";
      case ASK_SIDE: return "AskSide";
    }
    return null;
  }

  static Side fromString(String s) {
    if(s == null) return null;
    switch(s) {
      case "BidSide": return BID_SIDE;
      case "AskSide": return ASK_SIDE;
      default: return null;
    }
  }

  int toJson() => value;
  static Side fromJson(int v) {
    return v==null? null : values[v];
  }

}

class LogType implements Comparable<LogType> {
  static const LOG_BOOK = const LogType._(0);

  static get values => [
    LOG_BOOK
  ];

  final int value;

  int get hashCode => value;

  const LogType._(this.value);

  copy() => this;

  int compareTo(LogType other) => value.compareTo(other.value);

  String toString() {
    switch(this) {
      case LOG_BOOK: return "LogBook";
    }
    return null;
  }

  static LogType fromString(String s) {
    if(s == null) return null;
    switch(s) {
      case "LogBook": return LOG_BOOK;
      default: return null;
    }
  }

  int toJson() => value;
  static LogType fromJson(int v) {
    return v==null? null : values[v];
  }

}

class CreateMarketReq {
  const CreateMarketReq(this.reqId, this.userId, this.name, this.startTime,
    this.endTime, this.decimalShift, this.tickSize);

  final int reqId;
  final int userId;
  final String name;
  /// seconds since epoch indicating start time
  final int startTime;
  /// seconds since epoch indicating end time
  final int endTime;
  final int decimalShift;
  final int tickSize;
  // custom <class CreateMarketReq>
  // end <class CreateMarketReq>

  Map toJson() => {
      "req_id": ebisu_utils.toJson(reqId),
      "user_id": ebisu_utils.toJson(userId),
      "name": ebisu_utils.toJson(name),
      "start_time": ebisu_utils.toJson(startTime),
      "end_time": ebisu_utils.toJson(endTime),
      "decimal_shift": ebisu_utils.toJson(decimalShift),
      "tick_size": ebisu_utils.toJson(tickSize),
  };

  static CreateMarketReq fromJson(Object json) {
    if(json == null) return null;
    if(json is String) {
      json = convert.JSON.decode(json);
    }
    assert(json is Map);
    return new CreateMarketReq._fromJsonMapImpl(json);
  }

  CreateMarketReq._fromJsonMapImpl(Map jsonMap) :
    reqId = jsonMap["req_id"],
    userId = jsonMap["user_id"],
    name = jsonMap["name"],
    startTime = jsonMap["start_time"],
    endTime = jsonMap["end_time"],
    decimalShift = jsonMap["decimal_shift"],
    tickSize = jsonMap["tick_size"];

  CreateMarketReq._copy(CreateMarketReq other) :
    reqId = other.reqId,
    userId = other.userId,
    name = other.name,
    startTime = other.startTime,
    endTime = other.endTime,
    decimalShift = other.decimalShift,
    tickSize = other.tickSize;

}

class SubmitReq {
  const SubmitReq(this.reqId, this.userId, this.marketId, this.side, this.price,
    this.quantity);

  final int reqId;
  final int userId;
  final int marketId;
  final Side side;
  final int price;
  final int quantity;
  // custom <class SubmitReq>
  // end <class SubmitReq>

  Map toJson() => {
      "req_id": ebisu_utils.toJson(reqId),
      "user_id": ebisu_utils.toJson(userId),
      "market_id": ebisu_utils.toJson(marketId),
      "side": ebisu_utils.toJson(side),
      "price": ebisu_utils.toJson(price),
      "quantity": ebisu_utils.toJson(quantity),
  };

  static SubmitReq fromJson(Object json) {
    if(json == null) return null;
    if(json is String) {
      json = convert.JSON.decode(json);
    }
    assert(json is Map);
    return new SubmitReq._fromJsonMapImpl(json);
  }

  SubmitReq._fromJsonMapImpl(Map jsonMap) :
    reqId = jsonMap["req_id"],
    userId = jsonMap["user_id"],
    marketId = jsonMap["market_id"],
    side = Side.fromJson(jsonMap["side"]),
    price = jsonMap["price"],
    quantity = jsonMap["quantity"];

  SubmitReq._copy(SubmitReq other) :
    reqId = other.reqId,
    userId = other.userId,
    marketId = other.marketId,
    side = other.side == null? null : other.side.copy(),
    price = other.price,
    quantity = other.quantity;

}

class CancelReq {
  const CancelReq(this.reqId, this.userId, this.marketId, this.orderId);

  final int reqId;
  final int userId;
  final int marketId;
  final int orderId;
  // custom <class CancelReq>
  // end <class CancelReq>

  Map toJson() => {
      "req_id": ebisu_utils.toJson(reqId),
      "user_id": ebisu_utils.toJson(userId),
      "market_id": ebisu_utils.toJson(marketId),
      "order_id": ebisu_utils.toJson(orderId),
  };

  static CancelReq fromJson(Object json) {
    if(json == null) return null;
    if(json is String) {
      json = convert.JSON.decode(json);
    }
    assert(json is Map);
    return new CancelReq._fromJsonMapImpl(json);
  }

  CancelReq._fromJsonMapImpl(Map jsonMap) :
    reqId = jsonMap["req_id"],
    userId = jsonMap["user_id"],
    marketId = jsonMap["market_id"],
    orderId = jsonMap["order_id"];

  CancelReq._copy(CancelReq other) :
    reqId = other.reqId,
    userId = other.userId,
    marketId = other.marketId,
    orderId = other.orderId;

}

class ReplaceReq {
  const ReplaceReq(this.reqId, this.userId, this.marketId, this.orderId,
    this.price, this.quantity);

  final int reqId;
  final int userId;
  final int marketId;
  final int orderId;
  final int price;
  final int quantity;
  // custom <class ReplaceReq>
  // end <class ReplaceReq>

  Map toJson() => {
      "req_id": ebisu_utils.toJson(reqId),
      "user_id": ebisu_utils.toJson(userId),
      "market_id": ebisu_utils.toJson(marketId),
      "order_id": ebisu_utils.toJson(orderId),
      "price": ebisu_utils.toJson(price),
      "quantity": ebisu_utils.toJson(quantity),
  };

  static ReplaceReq fromJson(Object json) {
    if(json == null) return null;
    if(json is String) {
      json = convert.JSON.decode(json);
    }
    assert(json is Map);
    return new ReplaceReq._fromJsonMapImpl(json);
  }

  ReplaceReq._fromJsonMapImpl(Map jsonMap) :
    reqId = jsonMap["req_id"],
    userId = jsonMap["user_id"],
    marketId = jsonMap["market_id"],
    orderId = jsonMap["order_id"],
    price = jsonMap["price"],
    quantity = jsonMap["quantity"];

  ReplaceReq._copy(ReplaceReq other) :
    reqId = other.reqId,
    userId = other.userId,
    marketId = other.marketId,
    orderId = other.orderId,
    price = other.price,
    quantity = other.quantity;

}

class MarketDetailsReq {
  const MarketDetailsReq(this.reqId, this.marketId, this.includeActive,
    this.includeDead, this.includeFills);

  final int reqId;
  final int marketId;
  final bool includeActive;
  final bool includeDead;
  final bool includeFills;
  // custom <class MarketDetailsReq>
  // end <class MarketDetailsReq>

  Map toJson() => {
      "req_id": ebisu_utils.toJson(reqId),
      "market_id": ebisu_utils.toJson(marketId),
      "include_active": ebisu_utils.toJson(includeActive),
      "include_dead": ebisu_utils.toJson(includeDead),
      "include_fills": ebisu_utils.toJson(includeFills),
  };

  static MarketDetailsReq fromJson(Object json) {
    if(json == null) return null;
    if(json is String) {
      json = convert.JSON.decode(json);
    }
    assert(json is Map);
    return new MarketDetailsReq._fromJsonMapImpl(json);
  }

  MarketDetailsReq._fromJsonMapImpl(Map jsonMap) :
    reqId = jsonMap["req_id"],
    marketId = jsonMap["market_id"],
    includeActive = jsonMap["include_active"],
    includeDead = jsonMap["include_dead"],
    includeFills = jsonMap["include_fills"];

  MarketDetailsReq._copy(MarketDetailsReq other) :
    reqId = other.reqId,
    marketId = other.marketId,
    includeActive = other.includeActive,
    includeDead = other.includeDead,
    includeFills = other.includeFills;

}

class LogReq {
  const LogReq(this.logType, this.marketId);

  final LogType logType;
  final int marketId;
  // custom <class LogReq>
  // end <class LogReq>

  Map toJson() => {
      "log_type": ebisu_utils.toJson(logType),
      "market_id": ebisu_utils.toJson(marketId),
  };

  static LogReq fromJson(Object json) {
    if(json == null) return null;
    if(json is String) {
      json = convert.JSON.decode(json);
    }
    assert(json is Map);
    return new LogReq._fromJsonMapImpl(json);
  }

  LogReq._fromJsonMapImpl(Map jsonMap) :
    logType = LogType.fromJson(jsonMap["log_type"]),
    marketId = jsonMap["market_id"];

  LogReq._copy(LogReq other) :
    logType = other.logType == null? null : other.logType.copy(),
    marketId = other.marketId;

}
// custom <part requests>

CreateMarketReq createMarketReq(int reqId, int userId,
    String name, DateTime startTime, DateTime endTime,
    int decimalShift, int tickSize) =>
  new CreateMarketReq(reqId, userId, name,
      210866803200000000 + startTime.millisecondsSinceEpoch * 1000,
      210866803200000000 + endTime.millisecondsSinceEpoch * 1000,
      decimalShift, tickSize);

// end <part requests>
