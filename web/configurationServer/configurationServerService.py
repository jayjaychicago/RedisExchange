import sys, time
import redis
import json
import daemon
import string
import random
from daemon import daemon

class MyDaemon(daemon):
        def run(self):
		rc = redis.Redis()
		ps = rc.pubsub()
		ps.subscribe(['newExchange', 'newProduct', 'productsByExchange','setProduct','productDetails'])

		for item in ps.listen():
			if item['type'] == 'message':
				channel = item['channel']
				data = item['data']
				if channel == "productDetails":
                                        jdata = json.loads(data)
                                        name = jdata["name"]
                                        exchange = jdata["exchange"]
					alreadyThere = rc.exists("productdetails:" + exchange + ":" + name)
					if alreadyThere == "0":
						rc.publish('PrivateMessageOUT:' + jdata["conn"], '{"msgType":"setProduct","result":"FAIL","msg":"The product ' + name + ' on exchange  ' + exchange + ' doesnt exist"}')
                                        else:
						resultStr = '{"msgType":"productsDetails", "result":"SUCCESS", '
						result = rc.hgetall("productdetails:" + exchange + ":" + name)
						counter = 0
						for product in result:
							if product != "privateKey" and product != "conn":
								surplus = '"' + product + '":"' + result[product] + '"'
								if counter > 0:
									surplus = "," + surplus
								resultStr = resultStr + surplus
								counter = counter + 1
						resultStr = resultStr + "}"
						rc.publish('PrivateMessageOUT:' + jdata["conn"], resultStr)

				if channel == "setProduct":
					jdata = json.loads(data)
					name = jdata["name"]
					exchange = jdata["exchange"]
					privateKey = jdata["privateKey"]
					Iproperty = jdata["property"]
					Ivalue = jdata["value"]
					alreadyThere = rc.exists("productdetails:" + exchange + ":" + name)
					if alreadyThere == "0":
						rc.publish('PrivateMessageOUT:' + jdata["conn"], '{"msgType":"setProduct","result":"FAIL","msg":"The product ' + name + ' on exchange  ' + exchange + ' doesnt exist"}')
					else:
						rc.hset("productdetails:" + exchange + ":" + name,Iproperty,Ivalue)
						rc.publish('PrivateMessageOUT:' + jdata["conn"], '{"msgType":"setProduct","result":"SUCCESS","msg":"The product ' + name + ' on exchange  ' + exchange + ' had its property ' + Iproperty + ' changed to value ' + Ivalue + '"}')

				if channel == "newProduct":
					print "A new product request is being made"
					jdata = json.loads(data)
					name = jdata["name"]
					exchange = jdata["exchange"]
					privateKey = jdata["privateKey"]
					alreadyThere = rc.sadd('products', exchange + ':' + name)
					more = rc.sadd('productsbyexchange:' + exchange, name)
					if alreadyThere == 0:
						rc.publish('PrivateMessageOUT:' + jdata["conn"], '{"msgType":"newProduct","result":"FAIL","msg":"Exchange ' + exchange + ' already has a product named  ' + name + '"}')
					else:
						rc.hmset('productdetails:' + exchange + ':' + name, jdata)
						rc.publish('PrivateMessageOUT:' + jdata["conn"], '{"msgType":"newProduct","product":"' + name + '","result":"SUCCESS","msg":"Product name ' + name + ' created"}')
				if channel == "newExchange":
					print "A new exchange request is being made"
					jdata = json.loads(data)
					name = jdata["name"]
					alreadyThere = rc.sadd('exchanges', name)
					if alreadyThere == 0:
						rc.publish('PrivateMessageOUT:' + jdata["conn"], '{"msgType":"newExchange","result":"FAIL","msg":"This exchange name is already taken ' + name + ' already exist"}')
					else:
						privateKey = ''.join(random.choice(string.ascii_lowercase + string.digits) for _ in range(40))
						rc.hmset('exchangedetails:' + name, jdata)
						rc.set('exchangekey:' + name, privateKey)
						rc.publish('PrivateMessageOUT:' + jdata["conn"], '{"msgType":"newExchange","exchange":"' + name + '","result":"SUCCESS","msg":"Exchange name ' + name + ' created","PrivateKey":"' + privateKey + '"}')
				if channel == "productsByExchange":
					print "A list of all products for an exchange is being asked"
					jdata = json.loads(data)
					exchange = jdata["exchange"]
					token = jdata["token"]
					result = rc.smembers('productsbyexchange:' + exchange)	
					counter = 0
					resultStr = '{"msgType":"productsByExchange", "PRODUCTS": ['
					for product in result:
						surplus = '{"product":"' + product + '"}'
						if counter > 0:
							surplus = "," + surplus
						resultStr = resultStr + surplus
						counter = counter + 1
					resultStr = resultStr + "]}" 
					rc.publish('PrivateMessageOUT:' + jdata["conn"], resultStr)
if __name__ == "__main__":
        daemon = MyDaemon('/tmp/daemon-example.pid')
        if len(sys.argv) == 2:
                if 'start' == sys.argv[1]:
                        daemon.start()
                elif 'stop' == sys.argv[1]:
                        daemon.stop()
                elif 'restart' == sys.argv[1]:
                        daemon.restart()
                else:
                        print "Unknown command"
                        sys.exit(2)
                sys.exit(0)
        else:
                print "usage: %s start|stop|restart" % sys.argv[0]
                sys.exit(2)
