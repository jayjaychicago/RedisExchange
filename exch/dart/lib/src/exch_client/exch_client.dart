part of exch_client.exch_client;

class ExchClient {
  String host;
  int port;
  // custom <class ExchClient>

  ExchClient._ctor(this._publishClient, this._listenerClient);

  _startListening() {
    final channels = [
      'EX_RESP*',
    ];

    return _listenerClient.psubscribe(channels, onMessage);
  }

  onMessage(Receiver receiver) {
    receiver.receiveMultiBulkStrings().then((List message) {
      if(_requests.length > 0) {
        if(_logger.level <= Level.FINEST) {
          _logger.finest('Processed $message');
        }
        RequestCompleter completer = _requests.removeFirst();
        completer.complete(message[3]);
      } else {
        throw new Exception('Unexpected message: $message');
      }
    });
  }

  _nextResponse() async {

  }

  static makeClient(String host, int port) async {
    final connectString = '$host:$port';
    final publishClient = await RedisClient.connect(connectString);
    final listenerClient = await RedisClient.connect(connectString);
    final result = new ExchClient._ctor(publishClient, listenerClient);
    int rc = await result._startListening();
    return result;
  }

  close() {
    _publishClient.close();
    _listenerClient.close();
  }

  get _reqId => _nextReqId++;

  addRequestCompleter(Request request) {
    if(_logger.level <= Level.FINEST) {
      _logger.finest('Adding completer: $request');
    }
    final completer = new Completer();
    _requests.add(new RequestCompleter(request, completer));
    return completer.future;
  }

  createMarket(int userId, String marketName,
      DateTime startTime, DateTime endTime, {int decimalShift : 2, int tickSize : 1}) async {
    final request = createMarketReq(_reqId, userId, marketName,
        startTime, endTime, decimalShift, tickSize);
    final result = addRequestCompleter(request);
    await _publishClient.publish("EX_REQ:M", convert.JSON.encode(request));
    return result;
  }

  submit(int userId, int marketId, bool isAsk, int price, int quantity) async {
    final request = new SubmitReq(_reqId, userId, marketId,
        isAsk? Side.ASK_SIDE : Side.BID_SIDE,
        price, quantity);
    final result = addRequestCompleter(request);
    await _publishClient.publish("EX_REQ:S", convert.JSON.encode(request));
    return result;
  }

  cancel(int userId, int marketId, int orderId) async {
    final request = new CancelReq(_reqId, userId, marketId, orderId);
    final result = addRequestCompleter(request);
    await _publishClient.publish("EX_REQ:C", convert.JSON.encode(request.toJson()));
    return result;
  }

  replace(ReplaceReq req) =>
    _publishClient.publish("EX_REQ:R",
        convert.JSON.encode(req.toJson()));

  marketDetails(int marketId, bool includeActive, bool includeDead, bool includeFills) async {
    final request = new MarketDetailsReq(
        _reqId, marketId, includeActive, includeDead, includeFills);
    final result = addRequestCompleter(request);
    await _publishClient.publish("EX_REQ:D", convert.JSON.encode(request.toJson()));
    return result;
  }

  logBook(int marketId) async {
    return await _publishClient.publish("EX_REQ:L",
        convert.JSON.encode(new LogReq(LogType.LOG_BOOK, marketId)));
  }

  halt() => _publishClient.publish("EX_REQ:H", "Halt");


  // end <class ExchClient>
  RedisClient _publishClient;
  RedisClient _listenerClient;
  int _nextReqId = 1;
  Queue<RequestCompleter> _requests = new Queue();
}
// custom <part exch_client>
// end <part exch_client>
