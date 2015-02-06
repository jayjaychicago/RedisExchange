require 'uri'
require 'net/http'

privateKey = "123456789012345678901234567890"

def newExchange(email, password, name)
	uri = URI("http://api.instabid.io:3000/exchanges/")
	https = Net::HTTP.new(uri.host, uri.port)
#	https.use_ssl = true

	request = Net::HTTP::Post.new(uri.path)

	request["email"] = email 
	request["password"] = password 
	request["name"] = name 

	response = https.request(request)
	return response.body
end


def newProduct(exchange, name)
	uri = URI("http://api.instabid.io:3000/products/")
	https = Net::HTTP.new(uri.host, uri.port)
#	https.use_ssl = true

	request = Net::HTTP::Post.new(uri.path)

	request["exchange"] = exchange
	request["privateKey"] = privateKey 
	request["name"] = name 

	response = https.request(request)
	return response.body
end
