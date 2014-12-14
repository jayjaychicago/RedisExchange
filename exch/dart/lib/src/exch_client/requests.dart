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

}

class CreateMarketReq {
  const CreateMarketReq(this.reqId, this.userId, this.name, this.startTime,
    this.endTime, this.decimalShift, this.tickSize);

  final int reqId;
  final int userId;
  final int name;
  final String startTime;
  final String endTime;
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
  const SubmitReq(this.reqId, this.marketId, this.userId, this.side, this.price,
    this.quantity);

  final int reqId;
  final int marketId;
  final int userId;
  final Side side;
  final int price;
  final int quantity;
  // custom <class SubmitReq>
  // end <class SubmitReq>

  Map toJson() => {
      "req_id": ebisu_utils.toJson(reqId),
      "market_id": ebisu_utils.toJson(marketId),
      "user_id": ebisu_utils.toJson(userId),
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
    marketId = jsonMap["market_id"],
    userId = jsonMap["user_id"],
    side = Side.fromJson(jsonMap["side"]),
    price = jsonMap["price"],
    quantity = jsonMap["quantity"];

  SubmitReq._copy(SubmitReq other) :
    reqId = other.reqId,
    marketId = other.marketId,
    userId = other.userId,
    side = other.side == null? null : other.side.copy(),
    price = other.price,
    quantity = other.quantity;

}

class CancelReq {
  const CancelReq(this.reqId, this.marketId, this.userId, this.orderId);

  final int reqId;
  final int marketId;
  final int userId;
  final int orderId;
  // custom <class CancelReq>
  // end <class CancelReq>

  Map toJson() => {
      "req_id": ebisu_utils.toJson(reqId),
      "market_id": ebisu_utils.toJson(marketId),
      "user_id": ebisu_utils.toJson(userId),
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
    marketId = jsonMap["market_id"],
    userId = jsonMap["user_id"],
    orderId = jsonMap["order_id"];

  CancelReq._copy(CancelReq other) :
    reqId = other.reqId,
    marketId = other.marketId,
    userId = other.userId,
    orderId = other.orderId;

}

class ReplaceReq {
  const ReplaceReq(this.reqId, this.marketId, this.userId, this.existingOrderId,
    this.price, this.quantity);

  final int reqId;
  final int marketId;
  final int userId;
  final int existingOrderId;
  final int price;
  final int quantity;
  // custom <class ReplaceReq>
  // end <class ReplaceReq>

  Map toJson() => {
      "req_id": ebisu_utils.toJson(reqId),
      "market_id": ebisu_utils.toJson(marketId),
      "user_id": ebisu_utils.toJson(userId),
      "existing_order_id": ebisu_utils.toJson(existingOrderId),
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
    marketId = jsonMap["market_id"],
    userId = jsonMap["user_id"],
    existingOrderId = jsonMap["existing_order_id"],
    price = jsonMap["price"],
    quantity = jsonMap["quantity"];

  ReplaceReq._copy(ReplaceReq other) :
    reqId = other.reqId,
    marketId = other.marketId,
    userId = other.userId,
    existingOrderId = other.existingOrderId,
    price = other.price,
    quantity = other.quantity;

}
// custom <part requests>
// end <part requests>
