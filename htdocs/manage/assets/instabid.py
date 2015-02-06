# you need to do sudo pip install requests to use this library
import requests

privateKey = "123456789012345678901234567890"

def newExchange(email, password, name):
	payload = {'email': email, 'password': password, 'name': name}
	r = requests.post("http://api.instabid.io:3000/exchanges/", data=payload)
	return r.text 

def newExchange(exchange, privateKey, name):
	payload = {'exchange': exchange, 'privateKey': privateKey, 'name': name}
	r = requests.post("http://api.instabid.io:3000/products/", data=payload)
	return r.text 

