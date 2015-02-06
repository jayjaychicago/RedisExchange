import redis
import json
import daemon
import string
import random

rc = redis.Redis()
result = rc.hgetall('test')
counter = 0
resultStr = '{"msgType":"productsByExchange", "PRODUCTS": ['
for product in result:
	surplus = '{"' + product + '":"' + result[product] + '"}'
	if counter > 0:
		surplus = "," + surplus
	resultStr = resultStr + surplus
	counter = counter + 1
resultStr = resultStr + "]}"
print resultStr
