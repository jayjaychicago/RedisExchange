part of exch_client.exch_client;

class ExchClient {
  const ExchClient(this._redisClient);

  // custom <class ExchClient>

  createMarket(CreateMarketReq req) =>
    _redisClient.publish("EX_REQ:M",
        convert.JSON.encode(req.toJson()));

  submit(SubmitReq req) =>
    _redisClient.publish("EX_REQ:S",
        convert.JSON.encode(req.toJson()));

  cancel(CancelReq req) =>
    _redisClient.publish("EX_REQ:C",
        convert.JSON.encode(req.toJson()));

  replace(ReplaceReq req) =>
    _redisClient.publish("EX_REQ:R",
        convert.JSON.encode(req.toJson()));

  marketDetails(MarketDetailsReq req) =>
    _redisClient.publish("EX_REQ:D",
        convert.JSON.encode(req.toJson()));

  log(LogReq req) =>
    _redisClient.publish("EX_REQ:L",
        convert.JSON.encode(req.toJson()));

  halt() => _redisClient.publish("EX_REQ:H", "Halt");


  // end <class ExchClient>
  final RedisClient _redisClient;
}
// custom <part exch_client>
// end <part exch_client>
