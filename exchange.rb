#require 'rubygems'
require 'redis'
require 'json'
require 'daemons'

def cancel(object,oId,volCancel,userId)
 matchingSingleOldOrder = $redis2.hmget(object + ':ORDER:DETAILS:' + oId, 'qty', 'qtyfilled','price')
 if matchingSingleOldOrder.count != 0 then
	oldQty = matchingSingleOldOrder[0].to_i
        oldQtyFilled = matchingSingleOldOrder[1].to_i
        oldPrice = matchingSingleOldOrder[2].to_f

	newQty = [oldQtyFilled, oldQty - volCancel.to_i].max
	$redis2.hset(object + ':ORDER:DETAILS:' + oId, 'qty', newQty)
	change_depth(object, oldPrice, newQty-oldQty, userId)
 end
end

def publish_smallest_depth(object)
	#puts "A depth line was destroyed so we're scanning for smallest depth of " + object
	minscore = $redis2.ZREVRANGEBYSCORE(object + ':DEPTHS', 0, '-INF', "WITHSCORES") #:limit => [0, 1], :with_scores => true)
	if minscore.count != 0
		#puts "smallest ask found: " + minscore[1]
		$redis2.publish('SimplePriceFeed:' + object, '{"obj":"' + object +'","msgType":"S","P":"' +  minscore[1].to_f.abs.to_s + '"}')
		qty = $redis2.get(object + ':DEPTH:QTY:' + minscore[0])
		#puts "Corresponding qty for lowest ask: " + qty.to_s
		$redis2.publish('SimpleQtyFeed:' + object, '{"obj":"' + object +'","msgType":"QS","V":"' + qty.to_s + '"}')
                $redis2.set(object + ':SMALLESTASKVOL', qty.to_i.abs)
                $redis2.set(object + ':SMALLESTASK', minscore[1].to_f.abs.to_s)
	else
		$redis2.publish('SimplePriceFeed:' + object, '{"obj":"' + object +'","msgType":"S","P":"-"}')
		$redis2.publish('SimpleQtyFeed:' + object, '{"obj":"' + object +'","msgType":"QS","V":"-"}')
                $redis2.del(object + ':SMALLESTASKVOL')
                $redis2.del(object + ':SMALLESTASK')
	end
end

def publish_highest_depth(object)
	#puts "A depth line was destroyed so we're asking for highest depth of " + object
	minscore = $redis2.ZREVRANGEBYSCORE(object + ':DEPTHS', 'INF', 0, "WITHSCORES") #:limit => [0, 1], :with_scores => true)
	#puts "out"
	if minscore.count != 0
		#puts "highest bid found: " + minscore[1]
		$redis2.publish('SimplePriceFeedB:' + object, '{"obj":"' + object +'","msgType":"B","P":"' + minscore[1].to_f.abs.to_s + '"}')
		qty = $redis2.get(object + ':DEPTH:QTY:' + minscore[0])
		#puts "Corresponding qty for highest bid: " + qty.to_s
		$redis2.publish('SimpleQtyFeedB:' + object, '{"obj":"' + object +'","msgType":"QB","V":"' + qty.to_s + '"}')
                $redis2.set(object + ':HIGHESTBIDVOL', qty.to_i.abs)
                $redis2.set(object + ':HIGHESTBID',minscore[1].to_f.abs.to_s)
	else
		$redis2.publish('SimplePriceFeedB:' + object, '{"obj":"' + object +'","msgType":"B","P":"-"}')
		$redis2.publish('SimpleQtyFeedB:' + object, '{"obj":"' + object +'","msgType":"QB","V":"-"}')
                $redis2.del(object + ':HIGHESTBIDVOL')
                $redis2.del(object + ':HIGHESTBID')
	end
end

def publish_depth(object,depthNum, depthPrice,volume)
	#puts "Publishing depth of " + object.to_s + " for depth price " + depthPrice.to_s + " is: " + volume.to_s
		$redis2.publish('PriceFeed:' + object.to_s, '{"obj":"' + object +'","msgType":"PF","P":"' + depthPrice.to_s + '","V":"' + volume.to_s + '","N":"' + depthNum.to_s + '"}')
end

def change_depth(object, price, qty_change, user)
	#puts "Changing depth of price " + price.to_s + ' by ' + qty_change.to_s
	depthGrid = $redis2.ZRANGEBYSCORE(object + ':DEPTHS', price, price)
	if depthGrid.count == 0 then
		# no previous depth found
		newDepthNum = $redis2.incr(object + ':DEPTHS:NUM')
		$redis2.zadd(object + ':DEPTHS', price, newDepthNum)
		$redis2.set(object + ':DEPTH:QTY:' + newDepthNum.to_s, qty_change)
		publish_depth(object,newDepthNum,price,qty_change)
		final_depth = qty_change
		#uho issue: what if there is nothing in the depth: we need to update the lowest ask and highest bid and publish them
	else
		newDepthNum = depthGrid[0]
		qty = $redis2.get(object + ':DEPTH:QTY:' + newDepthNum)
		#puts "The existing depth qty is " + newDepthNum.to_s
		
		final_depth = $redis2.incrby(object + ':DEPTH:QTY:' + newDepthNum, qty_change)

		smallestAsk = -1* $redis2.get(object + ':SMALLESTASK').to_f
		highestBid = $redis2.get(object + ':HIGHESTBID')
		#puts "*********** The price depth which volume we are changing is " + price.to_s + " and the smallest ask is " + smallestAsk.to_s + " and the highest bid is " + highestBid.to_s + ". If these match, we should consider reducing/increasing the volume of the smallest/highest"
		if (smallestAsk.to_f == price.to_f) then
			$redis2.set(object + ':SMALLESTASKVOL', final_depth)
			$redis2.publish('SimpleQtyFeed:' + object, '{"obj":"' + object +'","msgType":"QS","V":"' + final_depth.to_s + '"}')
			#puts "set and published a new smallestaskvol of " + final_depth.to_s
		end				
		if (highestBid.to_f == price) then
			$redis2.set(object + ':HIGHESTBIDVOL', final_depth)
			$redis2.publish('SimpleQtyFeedB:' + object, '{"obj":"' + object +'","msgType":"QB","V":"' + final_depth.to_s + '"}')
			#puts "set and published a new highestbidvol of " + final_depth.to_s
		end				
	
		#puts "Final Depth of " + price.to_s + " is " + final_depth.to_s
		publish_depth(object,newDepthNum, price,final_depth)
		if final_depth == 0 then
			$redis2.del(object + ':DEPTH:QTY:' + newDepthNum)
			$redis2.zrem(object+ ':DEPTHS', newDepthNum)
			if price < 0 then #new
				$redis2.del(object + ':SMALLESTASK', price.abs)
				publish_smallest_depth(object)
			else #new
				$redis2.del(object + ':HIGHESTBID', price.abs) #new
				publish_highest_depth(object) #new
			end
			return
		end
	end

	#puts "Entering lowest price"	
	if price < 0 then
		#puts "were given negative price "+ price.to_s	
		lastSmallestPrice = $redis2.get(object + ':SMALLESTASK')
		if lastSmallestPrice != nil then
			#puts "Testing: " + price.abs.to_s + ' < ' + lastSmallestPrice.to_s
			if price.abs < lastSmallestPrice.to_f.abs then
				#puts "test positive publishing"
				$redis2.set(object + ':SMALLESTASK',price.abs)
				$redis2.publish('SimplePriceFeed:' + object, '{"obj":"' + object +'","msgType":"S","P":"' + price.abs.to_s + '"}' )
				$redis2.set(object + ':SMALLESTASKVOL', final_depth)
				$redis2.publish('SimpleQtyFeed:' + object, '{"obj":"' + object +'","msgType":"QS","V":"' + final_depth.to_s + '"}')
			end
		else
			#puts "did not find previous smallest so publishing this one"
			$redis2.set(object + ':SMALLESTASK',price.abs)
			$redis2.publish('SimplePriceFeed:' + object, '{"obj":"' + object +'","msgType":"S","P":"' + price.abs.to_s + '"}')
			$redis2.set(object + ':SMALLESTASKVOL', final_depth)
			$redis2.publish('SimpleQtyFeed:' + object, '{"obj":"' + object +'","msgType":"QS","V":"' + final_depth.to_s + '"}' )
		end
	end

	#puts "Entering highest price"	
	if price > 0 then
		#puts "were given positive price "+ price.to_s	
		lastHighestPrice = $redis2.get(object + ':HIGHESTBID')
		if lastHighestPrice != nil then
			#puts "Testing: " + price.abs.to_s + ' > ' + lastSmallestPrice.to_s
			if price.abs > lastHighestPrice.to_f.abs then
				#puts "test positive publishing"
				$redis2.set(object + ':HIGHESTBID',price.abs)
				$redis2.publish('SimplePriceFeedB:' + object, '{"obj":"' + object +'","msgType":"B","P":"' + price.abs.to_s + '"}')
				$redis2.set(object + ':HIGHESTBIDVOL', final_depth)
				$redis2.publish('SimpleQtyFeedB:' + object, '{"obj":"' + object +'","msgType":"QB","V":"' + final_depth.to_s + '"}')
			end
		else
			#puts "did not find previous smallest so publishing this one"
			$redis2.set(object + ':HIGHESTBID',price.abs)
			$redis2.publish('SimplePriceFeedB:' + object, '{"obj":"' + object +'","msgType":"B","P":"' + price.abs.to_s + '"}')
			$redis2.set(object + ':HIGHESTBIDVOL', final_depth)
			$redis2.publish('SimpleQtyFeedB:' + object, '{"obj":"' + object +'","msgType":"QB","V":"' + final_depth.to_s + '"}')
		end
	end
end

def record_close_position(user, object, qty, previousPosPrice, price)
	$redis2.ZINCRBY('CLOSEPOSBYOBJECT:USER:' + user, qty , object) #that will add to the previous amount that user has closed for that object 
	newClosePosNum = $redis2.INCR(object + ':CLOSEPOSITIONS:NUM')
	$redis2.SADD(object + ':CLOSEPOSITIONSBYUSER:' + user, newClosePosNum)
	$redis2.HMSET(object + ':CLOSEPOSITION:DETAILS:' + newClosePosNum.to_s, 'user', user, 'qty', qty, 'previousPosPrice', previousPosPrice, 'price', price) 
	#puts "Increasing the closed position for user " + user.to_s + " by " + qty.to_s;

	newQty = 0
	newPrice = 0	
	oldQty = 0
	oldPrice = 0
	if (qty>0) then
		result = $redis2.HMGET('AVGCLOSELONGPOSBYOBJECT:OBJ:' + object + ':USER:' + user, qty, price)
	else
		result = $redis2.HMGET('AVGCLOSESHORTPOSBYOBJECT:OBJ:' + object + ':USER:' + user, qty, price)
	end

	if (result.count>0) then
		oldQty = result[0].to_i
		oldPrice = result[1].to_f
		newPrice = ((oldQty * oldPrice) + (qty*price)) / (qty+oldQty)
		newQty = qty + oldQty 
	else
		newPrice = price
		newQty = qty
	end
	if (qty>0) then
		$redis2.HMSET('AVGCLOSELONGPOSBYOBJECT:OBJ:' + object + ':USER:' + user, 'qty', newQty, 'price', newPrice)
	else
		$redis2.HMSET('AVGCLOSESHORTPOSBYOBJECT:OBJ:' + object + ':USER:' + user, 'qty', newQty, 'price', newPrice)
	end
end

def record_open_position(user, object, qty, price)
	$redis2.ZINCRBY('OPENPOSBYOBJECT:USER:' + user, qty , object) #that will add to the previous amount that user has closed for that object 
	newQty = 0
	newPrice = 0	
	result = $redis2.HMGET('AVGOPENPOSBYOBJECT:OBJ:' + object + ':USER:' + user, qty, price)
	if (result.count>0) then
		oldQty = result[0].to_i
		oldPrice = result[1].to_f
		if ((oldQty<0) != (qty<0)) then
			#there has been an overall position flip for this user and this object. Wipe out the running average
			newQty = oldQty + qty
			newPrice = price #you were short 600 @ 20 and inserted an open pos long 1000 @ 30. You are left with a long 400 @ 30
		else
			newPrice = ((oldQty * oldPrice) + (qty*price)) / (qty+oldQty)
			newQty = qty + oldQty 
		end
	else
		newPrice = price
		newQty = qty
	end
	$redis2.HMSET('AVGOPENLONGPOSBYOBJECT:OBJ:' + object + ':USER:' + user, 'qty', newQty, 'price', newPrice)
	#puts "Increasing the open position for user " + user.to_s + " by " + qty.to_s;
end

def pnl(user, object, qty_close, previous_price, closing_price) 
	# user was long 25 lots(previous position) at $50(previous price). Closed that position at $60(closing price)
	#puts "User " + user.to_s + " had a position of " + qty_close.to_s + " in object " + object.to_s + " at " + previous_price.to_s + ". She closed that position at " + closing_price.to_s
	if qty_close.abs == 0 then return end
	#puts "Entering PNL storing"

	pnl = qty_close * (closing_price - previous_price)
	if pnl == 0 then return end
	newPnl = $redis2.incr(object + ':PNLS:NUM')
	#puts "pnl(" + pnl.to_s + ") = qty_close(" + qty_close.to_s  + ") * (closing_price(" + closing_price.to_s + ") - previous_price" + previous_price.to_s + ") )"
	#puts "Will increase PnL by " + pnl.to_s
	$redis2.hmset('PNLS:' + newPnl.to_s, 'user', user, 'qty_close', qty_close, 'previous_price', previous_price, 'closing_price', closing_price, 'pnl', pnl)
	totalPnl = $redis2.incrbyfloat('PNL:USER:' + user, pnl.to_f)

	totalPnlbyUserbyObject = $redis2.incrbyfloat('PNL:USER:' + user + ':OBJECT:' + object, pnl)

	$redis2.zadd('PNLBYUSER:OBJ:' + object, totalPnlbyUserbyObject, user)  #that will erase previous total pnl by user and allow to search by object to find users and their pnls
	$redis2.zadd('PNLBYOBJECT:USER:' + user, totalPnlbyUserbyObject, object) #that will erase previous total pnl by object and allow to search by user to find objects and their pnls 
	#puts "Done with PNL calculations"
end

def change_position(object, price, qty_change, user)
        #puts "********** Adding " + qty_change.to_s + " lots position at $" + price.to_s + ' by user ' + user 
	
	oldOverallPosition = $redis2.incrby(object + ':' + user + ':OVERALLPOSITION', qty_change) - qty_change #this does 2 actions: increase the overall position and stores its previous value

	if ((oldOverallPosition<0) == (qty_change<0) or oldOverallPosition ==0) then # if the new position is in the same direction as the old one NON OFFSETTING
		posGrid = $redis2.ZRANGEBYSCORE(object + ':POSITIONSBYUSER:' + user, price, price)
		previousPosCount = posGrid.count
		#puts "Found " + previousPosCount.to_s + " old positions IN THE SAME DIRECTION for " + object + ':POSITIONSBYUSER:' + user + ' matching the price score: ' + price.to_s
		if (previousPosCount == 0) then
                        #puts "It doesn't exist, let's add it"
                        newPosNum = $redis2.incr(object + ':POSITIONS:NUM')
                        $redis2.sadd(object + ':ALLPOSITIONS', newPosNum) #allows to collect all positions for one object: fails to make sense
			record_open_position(user, object, qty_change, price)
                        $redis2.zadd(object + ':POSITIONSBYUSER:' + user, price, newPosNum) #allow to collect all positions for one object for one user
                        $redis2.hmset(object + ':POSITION:DETAILS:' + newPosNum.to_s, 'qty', qty_change, 'user', user, 'price', price)
		else
			newPosNum = posGrid[0]	
			final_pos = $redis2.hincrby(object + ':POSITION:DETAILS:' + newPosNum,'qty', qty_change)
			record_open_position(user, object, qty_change, price)
		end
		return
	end
	
	# AFTER THIS IT IS ALWAYS OFFSETTING POSITIONS
	posGrid = $redis2.ZRANGEBYSCORE(object + ':POSITIONSBYUSER:' + user, "-INF", "+INF")
	qtyLeftToOffset = qty_change
	i=0
	previousPosCount = posGrid.count
	#puts "**************** Found " + previousPosCount.to_s + " offsetting positions in THE OPPOSITE direction"
	while (i<previousPosCount and qtyLeftToOffset.abs>0 and i<10000)
		if (i>9990) then
	                #puts "Caught in an infinite loop in the position calculator"
                end
		oldPosNum = posGrid[i]
		details = $redis2.hmget(object + ':POSITION:DETAILS:' + oldPosNum.to_s,'qty', 'price')
		previousPosQty = details[0].to_i
                previousPosPrice = details[1].to_f
		#puts "Comparing qtyLeftToOffset " + qtyLeftToOffset.abs.to_s + " >= " + previousPosQty.abs.to_s
		if (qtyLeftToOffset.abs >= previousPosQty.abs) then
			# that one position will get wiped out entirely
			$redis2.del(object + ':POSITION:DETAILS:' + oldPosNum)
                        $redis2.zrem(object+ ':POSITIONSBYUSER:' + user, oldPosNum)
			record_open_position(user, object, -1*previousPosQty, price)
			record_close_position(user,object, previousPosQty, previousPosPrice, price)
                        $redis2.srem(object+ ':ALLPOSITIONS', oldPosNum)
			qtyLeftToOffset = qtyLeftToOffset + previousPosQty
			#puts "Deleting old position which was " + previousPosQty.to_s + " lots at $" + previousPosPrice.to_s + "quantity left to offset down to " + qtyLeftToOffset.to_s
			pnl(user, object, previousPosQty, previousPosPrice, price)
			#puts "moving on..."
		else
			final_pos = $redis2.hincrby(object + ':POSITION:DETAILS:' + oldPosNum,'qty', qtyLeftToOffset)
			#puts "Changing old position to " + final_pos.to_s + ". It  was " + previousPosQty.to_s + " lots at $" + previousPosPrice.to_s
			record_close_position(user,object,qtyLeftToOffset, previousPosPrice, price)
			record_open_position(user,object,-1*qtyLeftToOffset, price)
			pnl(user, object, qtyLeftToOffset, previousPosPrice, price)
			#puts "moving on..."
			qtyLeftToOffset = 0 
		end
		i= i +1
	end
	
	if (qtyLeftToOffset.abs > 0) then
		# We have some position left to integrate
		newPosNum = $redis2.incr(object + ':POSITIONS:NUM')
                $redis2.sadd(object + ':ALLPOSITIONS', newPosNum) #allows to collect all positions for one object
                $redis2.zadd(object + ':POSITIONSBYUSER:' + user, price, newPosNum) #allow to collect all positions for one object for one user
                $redis2.hmset(object + ':POSITION:DETAILS:' + newPosNum.to_s, 'qty', qtyLeftToOffset, 'user', user, 'price', price)
		record_open_position(user, object, qtyLeftToOffset, price)
		#puts "Had some leftovers. So created a position: " + qtyLeftToOffset.to_s + " lots at $" + price.to_s
	end

	return
	#puts "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Should never enter this"
	
	posGrid = $redis2.ZRANGEBYSCORE(object + ':POSITIONSBYUSER:' + user, "-INF", "+INF")	
	#puts "Looking if the position for object " + object.to_s + " at price " + price.to_s + " exists already"
	previousPosCount = posGrid.count
	if previousPosCount == 0 then
			#puts "It doesn't exist, let's add it"
			newPosNum = $redis2.incr(object + ':POSITIONS:NUM')
			$redis2.sadd(object + ':ALLPOSITIONS', newPosNum) #allows to collect all positions for one object
			$redis2.zadd(object + ':POSITIONSBYUSER:' + user, price, newPosNum) #allow to collect all positions for one object for one user
                	$redis2.hmset(object + ':POSITION:DETAILS:' + newPosNum.to_s, 'qty', qty_change, 'user', user, 'price', price)
                #publish_position(object,newDepthNum,price,qty_change, user)
       	else
		i = 0
		qtyLeftToOffset = qty_change
		if ((previousPosQty<0) == (qtyLeftToOffset<0)) then
		
		else
			while (i<previousPosCount and qtyLeftToOffset.abs>0 and i<10000)
				if (i>9990) then  
					#puts "Caught in an infinite loop in the position calculator"
				end
	                	newPosNum = posGrid[i]
				# the next two lines are just for testing
       		         	details = $redis2.hmget(object + ':POSITION:DETAILS:' + newPosNum.to_s,'qty', 'price')
				previousPosQty = details[0].to_i
				previousPosPrice = details[1].to_f	
        	        	#puts "******* The existing position to alter is " + details[0].to_s + " lots at " + details[1].to_s
				#puts "******* The new position to assimilate is " + qty_change.to_s + " lots at " + price.to_s

				if (previousPosQty.abs>=qtyLeftToOffset.abs) then 
        	        		final_pos = $redis2.hincrby(object + ':POSITION:DETAILS:' + newPosNum,'qty', qtyLeftToOffset)
					#puts "******* I'm taking the old position " + previousPosQty.to_s + " and adding " + qtyLeftToOffset.to_s + " to it"
					qtyLeftToOffset = 0 
			
				else 
					$redis2.hmset(object + ':POSITION:DETAILS:' + newPosNum.to_s, 'qty', 0) 	
					#puts "******* I'm reducing the previous position to 0 because I had more in the new position that the old. Now I'm reducing the new position from " + qtyLeftToOffset.to_s + " to " + (qtyLeftToOffset + previousPosQty).to_s
					#toto = gets
					qtyLeftToOffset = qtyLeftToOffset + previousPosQty 		
					final_pos = 0 
				end
	       	         	#publish_depth(object,newDepthNum, price,final_depth)
        	        	if final_pos == 0 then
	                        	$redis2.del(object + ':POSITION:DETAILS:' + newPosNum)
	      	                  	$redis2.zrem(object+ ':POSITIONSBYUSER:' + user, newPosNum)
					$redis2.srem(object+ ':ALLPOSITIONS', newPosNum)
					if ($redis2.scard(object+ ':ALLPOSITIONS')==0) then $redis2.del(object+ ':ALLPOSITIONS') end
                       		 #publish_smallest_depth(object)
	                	end
				i = i + 1
			end
		end
        end

        #puts "Entering lowest price"
        #if price < 0
        #        puts "were given negative price "+ price.to_s
        ##        lastSmallestPrice = $redis2.get(object + ':SMALLESTASK')
        #        if lastSmallestPrice != nil then
        #                puts "Testing: " + price.abs.to_s + ' < ' + lastSmallestPrice.to_s
        #                if price.abs < lastSmallestPrice.to_f.abs then
        #                        puts "test positive publishing"
        #                        $redis2.set(object + ':SMALLESTASK',price.abs)
        #                        $redis2.publish('SimplePriceFeed:' + object, price.abs)
        #                end
        #        else
        #                puts "did not find previous smallest so publishing this one"
        #                $redis2.set(object + ':SMALLESTASK',price.abs)
        #                $redis2.publish('SimplePriceFeed:' + object, price.abs)
        #        end
        #end
end

def fill(newObjectId, priceMatch, qtyMatch, buyUserId, sellUserId)
	puts "Filling: " + newObjectId.to_s + " price: " + priceMatch.to_s + " qtyMatch: " + qtyMatch.to_s + " buyUserId: " + buyUserId.to_s + " seller: " + sellUserId 
        $redis2.publish('FillFeed:' + newObjectId + ':USER:' + buyUserId, '{"obj":"' + newObjectId +'","msgType":"F","V":"' + qtyMatch.to_s + '","P":"' + priceMatch.to_s + '","B":"' + buyUserId.to_s + '","S":"' + sellUserId.to_s + '"}')
        #puts "PUBLISH " + 'FillFeed:' + newObjectId + ':USER:' + buyUserId + ': {"obj":"' + newObjectId +'","msgType":"F","V":"' + qtyMatch.to_s + '","P":"' + priceMatch.to_s + '"}'
        #$redis2.publish('FillFeed:' + newObjectId + ':USER:' + sellUserId , '{"obj":"' + newObjectId +'","msgType":"F","V":"' + qtyMatch.to_s + '","P":"' + (-1*priceMatch.to_f).to_s + '","B":"' + buyUserId.to_s + '","S":"' + sellUserId.to_s + '"}')

	timestampFill = Time.now.utc
    	newFillNum = $redis2.incr(newObjectId + ':FILLS:NUM').to_s
	$redis2.hmset(newObjectId + ':FILLS:DETAILS:' + newFillNum, 'price_match', priceMatch.abs, 'qty_match', qtyMatch, 'buy_user_id', buyUserId, 'sell_user_id', sellUserId, 'object_id',newObjectId)
	change_position(newObjectId, priceMatch.abs, qtyMatch, buyUserId)
	change_position(newObjectId, priceMatch.abs, -1*qtyMatch, sellUserId)


	$redis2.ZADD(newObjectId + ':DAYSACTIVE', (format('%02d', timestampFill.year.to_s[0,3]) + format('%02d', timestampFill.month.to_s) + format('%02d', timestampFill.day.to_s)).to_i, format('%02d', timestampFill.year.to_s[0,3]) + format('%02d', timestampFill.month.to_s) + format('%02d', timestampFill.day.to_s))
	totalTodayVolForThisObject = $redis2.INCRBY(newObjectId + ':DAILYTOTALVOLUME:' + format('%02d', timestampFill.year.to_s[0,3]) + format('%02d', timestampFill.month.to_s) + format('%02d', timestampFill.day.to_s), qtyMatch.abs) 
	$redis2.publish('DailyTotalVolumeFeed:' + newObjectId, '{"obj":"' + newObjectId +'","msgType":"DTV","V":"' + totalTodayVolForThisObject.to_s + '"}')
	#formattedTimestampFill = timestampFill.year.to_s + timestampFill.month.to_s + timestampFill.day.to_s + timestampFill.hour.to_s + timestampFill.min.to_s + timestampFill.sec.to_s + timestampFill.usec.to_s
	formattedTimestampFill = format('%02d', timestampFill.year.to_s[2,3]) + format('%02d', timestampFill.month.to_s) + format('%02d', timestampFill.day.to_s) + format('%02d', timestampFill.hour.to_s) + format('%02d', timestampFill.min.to_s) + format('%02d', timestampFill.sec.to_s) + format('%05d', timestampFill.usec.to_s[0,4]) 
    	$redis2.zadd(newObjectId + ':FILLS', formattedTimestampFill, newFillNum)
	$redis2.zadd(newObjectId + ':FILLS:BYUSER:' + buyUserId.to_s, formattedTimestampFill, newFillNum)
	$redis2.zadd(newObjectId + ':FILLS:BYUSER:' + sellUserId.to_s, formattedTimestampFill, newFillNum)


	# hourly and daily data
        formattedTimestampFillByHour = format('%02d', timestampFill.year.to_s[2,3]) + format('%02d', timestampFill.month.to_s) + format('%02d', timestampFill.day.to_s) + format('%02d', timestampFill.hour.to_s)
        formattedTimestampFillByDay = format('%02d', timestampFill.year.to_s[2,3]) + format('%02d', timestampFill.month.to_s) + format('%02d', timestampFill.day.to_s) 

	hourDataPointDetails = $redis2.ZRANGEBYSCORE("HOURLYDATAPOINTS:" + newObjectId, formattedTimestampFillByHour, formattedTimestampFillByHour)
	if hourDataPointDetails.count != 0 then
		hourDataPointDetailsNum = hourDataPointDetails[0].to_s	
	else
                hourDataPointDetailsNum = $redis2.incr("HOURLYDATAPOINTSNUM").to_s
                $redis2.zadd("HOURLYDATAPOINTS:" + newObjectId, formattedTimestampFillByHour, hourDataPointDetailsNum)
	end 
	dayDataPointDetails = $redis2.ZRANGEBYSCORE("DAILYDATAPOINTS:" + newObjectId, formattedTimestampFillByDay, formattedTimestampFillByDay)
	if dayDataPointDetails.count != 0 then
		dayDataPointDetailsNum = dayDataPointDetails[0].to_s
	else
	        dayDataPointDetailsNum = $redis2.incr("DAILYDATAPOINTSNUM" ).to_s
                $redis2.zadd("DAILYDATAPOINTS:" + newObjectId, formattedTimestampFillByDay, dayDataPointDetailsNum)
	end

        topDay = $redis2.GET("LASTDAYTRADED:" + newObjectId)
        if topDay != formattedTimestampFillByDay then
                $redis2.HMSET("DAYDATAPOINTDETAILS:" + dayDataPointDetailsNum, "OPEN", priceMatch.abs)
                lastPrice = $redis2.get(newObjectId + ':LASTPRICE')
                $redis2.SET("LASTCLOSE:" + newObjectId, lastPrice)
                $redis2.SET("LASTDAYTRADED:" + newObjectId,formattedTimestampFillByDay)
        else
                $redis2.HMSET("DAYDATAPOINTDETAILS:" + dayDataPointDetailsNum, "CLOSE", priceMatch.abs)
        end

        $redis2.set(newObjectId + ':LASTPRICE', priceMatch.abs)
        $redis2.publish('LastPriceFeed:' + newObjectId, '{"obj":"' + newObjectId +'","msgType":"LP","P":"' + priceMatch.abs.to_s + '"}')

        newHourlyVolume = $redis2.HINCRBY("HOURDATAPOINTDETAILS:" + hourDataPointDetailsNum, "VOL", qtyMatch.abs)
	newDailyVolume = $redis2.HINCRBY("DAYDATAPOINTDETAILS:" + dayDataPointDetailsNum, "VOL", qtyMatch.abs)

	highLow = $redis2.HMGET("HOURDATAPOINTDETAILS:" + hourDataPointDetailsNum, "HIGH", "LOW")
	if highLow.count != 0 then
		 oldHigh = highLow[0].to_f
		 oldLow = highLow[1].to_f
	end

	if oldHigh.nil? or oldLow.nil? or oldHigh == 0 or oldLow == 0 then
		$redis2.HMSET("HOURDATAPOINTDETAILS:" + hourDataPointDetailsNum, "LOW", priceMatch.abs, "HIGH", priceMatch.abs)
	else
		if priceMatch.abs > oldHigh then
			$redis2.HSET("HOURDATAPOINTDETAILS:" + hourDataPointDetailsNum, "HIGH", priceMatch.abs)	
		else
			if priceMatch.abs < oldLow then
				$redis2.HSET("HOURDATAPOINTDETAILS:" + hourDataPointDetailsNum, "LOW", priceMatch.abs)	
			end
		end
	end
      
        highLow = $redis2.HMGET("DAYDATAPOINTDETAILS:" + dayDataPointDetailsNum, "HIGH", "LOW")
        if highLow.count != 0 then
                 oldHigh = highLow[0].to_f
                 oldLow = highLow[1].to_f
        end

        if oldHigh.nil? or oldLow.nil? or oldHigh == 0 or oldLow == 0 then
                $redis2.HMSET("DAYDATAPOINTDETAILS:" + dayDataPointDetailsNum, "LOW", priceMatch.abs, "HIGH", priceMatch.abs)
        else
                if priceMatch.abs > oldHigh then
                        $redis2.HSET("DAYDATAPOINTDETAILS:" + dayDataPointDetailsNum, "HIGH", priceMatch.abs)
                else
                        if priceMatch.abs < oldLow then
                                $redis2.HSET("DAYDATAPOINTDETAILS:" + dayDataPointDetailsNum, "LOW", priceMatch.abs)
                        end
                end
        end

	return newFillNum

end

def match_and_adjust_depth(newOrderNum, newSide, newQty, newPrice2, newObjectId, reverseTimestamp, newUserId)
	#puts "Matching: " + newObjectId.to_s
	newPrice = newPrice2.to_f.abs
	newQtyFilled = 0
	#puts "newPrice2 is: " + newPrice2.to_s
	change_depth(newObjectId, newPrice2, newQty, newUserId)
	if newSide == "B" then	
		#puts "Your buy order will match with the following sells by order of preference:"
		matchingOldOrders = $redis2.ZREVRANGEBYSCORE(newObjectId + ':ORDERS', 0, "-" + newPrice.to_s, 'WITHSCORES')
	else
		#puts "Your sell order will match with the following sells by order of preference:"
		matchingOldOrders = $redis2.ZREVRANGEBYSCORE(newObjectId + ':ORDERS', "+INF", newPrice.to_s, 'WITHSCORES')
	end

	if matchingOldOrders.count == 0 then
		#puts "No matching orders were found"
		#, newUserIdchange_depth(newObjectId, newPrice2, newQty)
		#matchingOldOrders = $redis2.zrangebyscore(newObjectId + ':DEPTHS', newPrice, newPrice)
		#newQty_already_in_depth = 0
		#if matchingOldOrders.count != 0 then
	#		newQty_already_in_depth = matchingOldOrders[0]
	#		puts "Found existing depth of " + newQty_already_in_depth.to_s
	#		puts "Updating it to " + (newQty_already_in_depth + newQty).to_s
	#		$redis2.zrem(newObjectId + ':DEPTH', newQty_already_in_depth)
	#	else
	#		puts "no matching depth was found"
	#	end
	#	$redis2.zadd(newObjectId + ':DEPTH', newPrice, newQty_already_in_depth + newQty) 
	else
		#puts "yeah! I found some matching orders"
		i = 0
		imax = matchingOldOrders.count
		jsonI = 0
		newFillJson = ""
		allFillsJson = ""
		while (i < imax and newQtyFilled<newQty) do
			oldFullOrderNum = matchingOldOrders[i]
			oldShortOrderNum = matchingOldOrders[i][matchingOldOrders[i].index("ORD")+3,matchingOldOrders[i].size]
			i = i + 1
			oldPrice = matchingOldOrders[i].to_f
			i = i + 1
			matchingSingleOldOrder = $redis2.hmget(newObjectId + ':ORDER:DETAILS:' + oldShortOrderNum, 'qty', 'qtyfilled', 'userid')
			if matchingSingleOldOrder.count != 0 then
				puts "found the details of my matching order"
				oldQty = matchingSingleOldOrder[0].to_i
				oldQtyFilled = matchingSingleOldOrder[1].to_i
				oldUserId = matchingSingleOldOrder[2].to_s
				#puts "Oid: " + oldShortOrderNum + " Price: " + oldPrice.to_s + ' qty: ' + oldQty.to_s + ' qtyFilled: ' + oldQtyFilled.to_s + ' oldUserId: ' + oldUserId
				if oldPrice >=0 then
					oldSide = "B"
				else
					oldSide = "S"
				end
				qtyMatchableOld = oldQty - oldQtyFilled
				qtyMatchableNew = newQty - newQtyFilled
				qtyMatch = [qtyMatchableOld, qtyMatchableNew].min
				if qtyMatchableOld <= 0 or qtyMatchableNew <= 0 then
					#puts "Not enough quantity to match"
				else
					#puts "We have a fill or a partial fill"
					if newSide == "B" then
						newFillNum = fill(newObjectId, oldPrice, qtyMatch, newUserId, oldUserId)	
					else
						newFillNum = fill(newObjectId, oldPrice, qtyMatch, oldUserId, newUserId)	
					end
					newFillJson = '{"fillNum":"' + newFillNum.to_s + '", "product":"' + newObjectId.to_s + '","price":"' + oldPrice.abs.to_s + '","qty":"' + qtyMatch.to_s + '","user1":"' + newUserId.to_s + '","user2":"' + oldUserId.to_s + '"}'
					if jsonI != 0 
						allFillsJson = allFillsJson + ' , '
					end
					jsonI = jsonI + 1
					allFillsJson = allFillsJson + newFillJson
					newQtyFilled = newQtyFilled+qtyMatch
					oldQtyFilled = oldQtyFilled+qtyMatch
					$redis2.hset(newObjectId + ':ORDER:DETAILS:' + newOrderNum, 'qtyfilled', newQtyFilled)
					$redis2.hset(newObjectId + ':ORDER:DETAILS:' + oldShortOrderNum, 'qtyfilled', oldQtyFilled)
					if oldPrice >= 0 then
						change_depth(newObjectId, oldPrice, -1*qtyMatch, newUserId)
						change_depth(newObjectId, -1*newPrice, -1*qtyMatch, newUserId)
					else
						change_depth(newObjectId, oldPrice, -1*qtyMatch, newUserId)
						change_depth(newObjectId, newPrice, -1*qtyMatch, newUserId)
					end
				end		 
			end






		end
	end
	
	allFillsJson = '{"fillCount":"' + jsonI.to_s + '","fills":[' + allFillsJson.to_s + ']}'
	return allFillsJson
end

$redis = Redis.new(:timeout => 0, :port => 6379)
$redis.select('15')

#$redis2 = Redis.connect
$redis2 = Redis.new(:timeout => 0, :port => 6379)
$redis2.select('15')

#$redis.psubscribe() do |on|
#end

$redis.psubscribe('ORDERS:*', 'PrivateMessageIN:*', 'CANCELS:*', 'GETDEPTH:*', 'GETORDER:*', 'GETFILL:*') do |on|

  on.pmessage do |pattern, event, msg|

  
  if (pattern == 'PrivateMessageIN:*')
    #puts "=========Received private message from user================="
    #puts event + ' ' + msg
    data = JSON.parse(msg)
    
    #puts "Received redis in message with messageType= " + data['messageType']

    if (data['messageType']=='RequestBestDepth')
    	#puts "Asking for smallest depth of " + data['objectId']
    	#minscore = $redis2.ZREVRANGEBYSCORE(data['objectId'] + ':DEPTHS', 0, '-INF', 'WITHSCORES')
	minscore = $redis2.get(data['objectId'] + ':SMALLESTASK').to_f
    	if ((minscore != 0) and (minscore != nil)) 
      		#puts "min depth found: " + minscore.to_s
      		$redis2.publish('PrivateMessageOUT:' + event[17..-1].to_s, '{"obj":"' + data['objectId'] +'","msgType":"S","P":"' + minscore.abs.to_s + '"}' )
      		#$redis2.publish('SimplePriceFeed:' + object, minscore[1].to_f.abs.to_s)
    	else
		#puts "no min depth found: " + minscore.to_s
        	$redis2.publish('PrivateMessageOUT:' + event[17..-1].to_s,'{"obj":"' + data['objectId'] +'","msgType":"S","P":"-"}')
        	#$redis2.publish('SimplePriceFeed:' + object, 'N/A')
    	end
    elsif (data['messageType']=='RequestBestDepthB')
        maxscore = $redis2.get(data['objectId'] + ':HIGHESTBID').to_f
        if ((maxscore != 0) and (maxscore != nil))
                #puts "min depth found: " + minscore[1]
                $redis2.publish('PrivateMessageOUT:' + event[17..-1],'{"obj":"' + data['objectId'] +'","msgType":"B","P":"' + maxscore.abs.to_s + '"}')
                #$redis2.publish('SimplePriceFeed:' + object, minscore[1].to_f.abs.to_s)
        else
                $redis2.publish('PrivateMessageOUT:' + event[17..-1].to_s,'{"obj":"' + data['objectId'] +'","msgType":"B","P":"-"}')
                #$redis2.publish('SimplePriceFeed:' + object, 'N/A')
        end
    elsif (data['messageType']=='RequestDailyTotalVolume') # TO BE CODED
	timestampFill = Time.now.utc
	maxscore = $redis2.get(data['objectId'] + ':DAILYTOTALVOLUME:' + format('%02d', timestampFill.year.to_s[0,3]) + format('%02d', timestampFill.month.to_s) + format('%02d', timestampFill.day.to_s))
        if ((maxscore != 0) and (maxscore !=nil)) 
                #puts "min depth found: " + minscore[1]
                $redis2.publish('PrivateMessageOUT:' + event[17..-1],'{"obj":"' + data['objectId'] +'","msgType":"DTV","V":"' + maxscore + '"}')
                #$redis2.publish('SimplePriceFeed:' + object, minscore[1].to_f.abs.to_s)
        else
                $redis2.publish('PrivateMessageOUT:' + event[17..-1].to_s,'{"obj":"' + data['objectId'] +'","msgType":"DTV","V":"-"}')
                #$redis2.publish('SimplePriceFeed:' + object, 'N/A')
        end
    elsif (data['messageType']=='RequestLastPrice') # TO BE CODED
        maxscore = $redis2.get(data['objectId'] + ':LASTPRICE').to_f
        if ((maxscore != 0) and (maxscore != nil)) 
                #puts "min depth found: " + minscore[1]
                $redis2.publish('PrivateMessageOUT:' + event[17..-1],'{"obj":"' + data['objectId'] +'","msgType":"LP","P":"' + maxscore.to_s + '"}')
                #$redis2.publish('SimplePriceFeed:' + object, minscore[1].to_f.abs.to_s)
        else
                $redis2.publish('PrivateMessageOUT:' + event[17..-1].to_s,'{"obj":"' + data['objectId'] +'","msgType":"LP","P":"-"}')
                #$redis2.publish('SimplePriceFeed:' + object, 'N/A')
        end
    elsif (data['messageType']=='RequestQtyFeed') # TO BE CODED
        maxscore = $redis2.get(data['objectId'] + ':SMALLESTASKVOL').to_i
        if ((maxscore != 0) and (maxscore != nil)) 
                #puts "min depth found: " + minscore[1]
                $redis2.publish('PrivateMessageOUT:' + event[17..-1],'{"obj":"' + data['objectId'] +'","msgType":"QS","V":"' + maxscore.abs.to_s + '"}')
                #$redis2.publish('SimplePriceFeed:' + object, minscore[1].to_f.abs.to_s)
        else
                $redis2.publish('PrivateMessageOUT:' + event[17..-1].to_s,'{"obj":"' + data['objectId'] +'","msgType":"QS","V":"-"}')
                #$redis2.publish('SimplePriceFeed:' + object, 'N/A')
        end
    elsif (data['messageType']=='RequestQtyFeedB') # TO BE CODED
        maxscore = $redis2.get(data['objectId'] + ':HIGHESTBIDVOL').to_i
        if ((maxscore != 0) and (maxscore != nil)) 
                #puts "min depth found: " + minscore[1]
                $redis2.publish('PrivateMessageOUT:' + event[17..-1],'{"obj":"' + data['objectId'] +'","msgType":"QB","V":"' + maxscore.abs.to_s + '"}')
                #$redis2.publish('SimplePriceFeed:' + object, minscore[1].to_f.abs.to_s)
        else
                $redis2.publish('PrivateMessageOUT:' + event[17..-1].to_s,'{"obj":"' + data['objectId'] +'","msgType":"QB","V":"-"}')
                #$redis2.publish('SimplePriceFeed:' + object, 'N/A')
        end
    else
	#puts "response to other private messages"
    end

  elsif (pattern == 'CANCELS:*')
	#puts "====== RECEIVED INSTRUCTION TO CANCEL AN ORDER ======"
        timestamp = Time.now.utc
        formattedTimestamp = format('%02d', timestamp.year.to_s[2,3]) + format('%02d', timestamp.month.to_s) + format('%02d', timestamp.day.to_s) + format('%02d', timestamp.hour.to_s) + format('%02d', timestamp.min.to_s) + format('%02d', timestamp.sec.to_s) + format('%05d', timestamp.usec.to_s[0,4])
        #formattedTimestamp = timestamp.year.to_s[2,3] + timestamp.month.to_s + timestamp.day.to_s + timestamp.hour.to_s + timestamp.min.to_s + timestamp.sec.to_s + timestamp.usec.to_s[0,4]
        reverseTimestamp = (999999999999999 - formattedTimestamp.to_i).to_s
	data = JSON.parse(msg)
	#puts "volCancel: #{data['volCancel']} and object: #{data['object']} and oId: #{data['oId']} and userId: #{data['userId']}"
        volCancel = data['volCancel']	
        object = data['object']	
        oId = data['oId']	
        userId = data['userId']
	cancel(object,oId,volCancel,userId)

  elsif (pattern == 'GETDEPTH:*')
	data = JSON.parse(msg)
	newConn = data['conn']
	productName = data['productName']
	exchangeName = data['exchangeName']
	level = data['level']

	depthGridS = $redis2.ZRANGEBYSCORE(exchangeName + ':' + productName + ':DEPTHS', "-INF", "0", 'WITHSCORES')
	depthGridB = $redis2.ZREVRANGEBYSCORE(exchangeName + ':' + productName + ':DEPTHS', "+INF", "0", 'WITHSCORES')
	depthFeedback = '{"msgType":"GETDEPTH", "SELLDEPTHS":['
        if depthGridS.count != 0 then
                # no previous depth found
		#depthGridS.each { |x| puts $redis2.get(exchangeName + ':' + productName + ':DEPTH:QTY:' + x.to_s) }
		i = 0
		while (i<depthGridS.count)
			if (i % 2 == 0)
				if (i != 0) then
					depthFeedback = depthFeedback + ","
				end
				depthNum = depthGridS[i]	
				#puts "Num: " + depthNum.to_s
				depthQty = $redis2.get(exchangeName + ':' + productName + ':DEPTH:QTY:' + depthNum.to_s)
				#puts "Qty: " + depthQty #$redis2.get(exchangeName + ':' + productName + ':DEPTH:QTY:' + depthNum.to_s)
				depthFeedback = depthFeedback + '{"NUM":"' + depthNum.to_s + '","QTY":"' + depthQty.to_s +  '", '
			else
				depthPrice = depthGridS[i]
				#puts "Price: " + depthPrice.to_s
				depthFeedback = depthFeedback + '"PRICE":"' + depthPrice.to_s + '"}'
			end
			i = i + 1
		end
	end
	depthFeedback = depthFeedback + "]"
	depthFeedback = depthFeedback + ', "BUYDEPTHS":['
	if depthGridB.count != 0 then
                # no previous depth found
                #depthGridS.each { |x| puts $redis2.get(exchangeName + ':' + productName + ':DEPTH:QTY:' + x.to_s) }
                i = 0
                while (i<depthGridB.count)
                        if (i % 2 == 0)
                                if (i != 0) then
                                        depthFeedback = depthFeedback + ","
                                end
                                depthNum = depthGridB[i]        
                                #puts "Num: " + depthNum.to_s
                                depthQty = $redis2.get(exchangeName + ':' + productName + ':DEPTH:QTY:' + depthNum.to_s)
                                #puts "Qty: " + depthQty #$redis2.get(exchangeName + ':' + productName + ':DEPTH:QTY:' + depthNum.to_s)
                                depthFeedback = depthFeedback + '{"NUM":"' + depthNum.to_s + '","QTY":"' + depthQty.to_s +  '", '
                        else
                                depthPrice = depthGridB[i]
                                #puts "Price: " + depthPrice.to_s
                                depthFeedback = depthFeedback + '"PRICE":"' + depthPrice.to_s + '"}'
                        end
                        i = i + 1
                end
        end
	depthFeedback = depthFeedback + '] }'
	#puts depthFeedback
	$redis2.publish('PrivateMessageOUT:' + newConn, depthFeedback)

  elsif (pattern == 'GETORDER:*')
        data = JSON.parse(msg)
        newConn = data['conn']
        productName = data['productName']
        exchangeName = data['exchangeName']
        level = data['level']
	user = data['user']

        orderGridS = $redis2.ZREVRANGEBYSCORE(exchangeName + ':' + productName + ':ORDERS:BYUSER:' + user, "+INF", "-INF", 'WITHSCORES')
        orderFeedback = '{"msgType":"GETORDER", "ORDERS":['
        if orderGridS.count != 0 then
                # no previous order found
                #orderGridS.each { |x| puts $redis2.get(exchangeName + ':' + productName + ':DEPTH:QTY:' + x.to_s) }
                i = 0
                while (i<orderGridS.count)
                        if (i % 2 == 0)
                                if (i != 0) then
                                        orderFeedback = orderFeedback + ","
                                end
                                orderNum = orderGridS[i]
                                #puts "Num: " + orderNum.to_s
                                orderDetails = $redis2.hmget(exchangeName + ':' + productName + ':ORDER:DETAILS:' + orderNum.to_s, 'qty', 'qtyfilled','price') 
				if orderDetails.count != 0 then
					orderQty = orderDetails[0].to_i
					orderQtyFilled = orderDetails[1].to_i
					orderPrice = orderDetails[2].to_f
				end
                                #puts "Qty: " + orderQty #$redis2.get(exchangeName + ':' + productName + ':DEPTH:QTY:' + orderNum.to_s)
                                orderFeedback = orderFeedback + '{"NUM":"' + orderNum.to_s + '","QTY":"' + orderQty.to_s +  '", "QTYFILLED":"' + orderQtyFilled.to_s + '", "PRICE":"' + orderPrice.to_s + '", '
                        else
                                orderTimeStamp = orderGridS[i]
                                #puts "Price: " + orderPrice.to_s
                                orderFeedback = orderFeedback + '"TIMESTAMP":"' + orderTimeStamp.to_s + '"}'
                        end
                        i = i + 1
                end
        end
        orderFeedback = orderFeedback + '] }'
        #puts orderFeedback
        $redis2.publish('PrivateMessageOUT:' + newConn, orderFeedback)

  elsif (pattern == 'GETFILL:*')
        data = JSON.parse(msg)
        newConn = data['conn']
        productName = data['productName']
        exchangeName = data['exchangeName']
        level = data['level']
        user = data['user']

        fillFeedback = '{"msgType":"GETFILL", "FILLS":['
        fillGridS = $redis2.ZREVRANGEBYSCORE(exchangeName + ':' + productName + ':FILLS:BYUSER:' + user, "+INF", "-INF", 'WITHSCORES')
	hadBuyers = 0
        if fillGridS.count != 0 then
		hadBuyers = 1
                # no previous order found
                #orderGridS.each { |x| puts $redis2.get(exchangeName + ':' + productName + ':DEPTH:QTY:' + x.to_s) }
                i = 0
                while (i<fillGridS.count)
                        if (i % 2 == 0)
                                if (i != 0) then
                                        fillFeedback = fillFeedback + ","
                                end
                                fillNum = fillGridS[i]
                                #puts "Num: " + orderNum.to_s
                                fillDetails = $redis2.hmget(exchangeName + ':' + productName + ':FILLS:DETAILS:' + fillNum.to_s, 'qty_match', 'price_match', 'buy_user_id', 'sell_user_id','object_id')
				#puts 'HMGET ' + exchangeName + ':' + productName + ':FILLS:DETAILS:' + fillNum.to_s + ' qty_match ' + 'price_match ' + 'buy_user_id ' + 'sell_user_id ' + 'object_id'
                                if fillDetails.count != 0 then
                                        fillQty = fillDetails[0].to_i
                                        fillPrice = fillDetails[1].to_f
                                        fillBuyer = fillDetails[2].to_s
                                        fillSeller = fillDetails[3].to_s
                                        fillProduct = fillDetails[4].to_s
                                end
                                #puts "Qty: " + orderQty #$redis2.get(exchangeName + ':' + productName + ':DEPTH:QTY:' + orderNum.to_s)
                                fillFeedback = fillFeedback + '{"NUM":"' + fillNum.to_s + '","QTY":"' + fillQty.to_s +  '", "PRICE":"' + fillPrice.to_s + '", "BUYER":"' + fillBuyer.to_s + '", "SELLER":"' + fillSeller.to_s + '", "PRODUCT":"' + fillProduct.to_s + '", '
                        else
                                fillTimeStamp = fillGridS[i]
                                #puts "Price: " + orderPrice.to_s
                                fillFeedback = fillFeedback + '"TIMESTAMP":"' + fillTimeStamp.to_s + '"}'
                        end
                        i = i + 1
                end
        end


        fillFeedback = fillFeedback + '] }'
        #puts fillFeedback
        $redis2.publish('PrivateMessageOUT:' + newConn, fillFeedback)

  elsif (pattern == 'ORDERS:*')
	#on.pmessage do |pattern, event, msg|
	#puts "====== RECEIVED NEW ORDER =================="
	timestamp = Time.now.utc
	formattedTimestamp = format('%02d', timestamp.year.to_s[2,3]) + format('%02d', timestamp.month.to_s) + format('%02d', timestamp.day.to_s) + format('%02d', timestamp.hour.to_s) + format('%02d', timestamp.min.to_s) + format('%02d', timestamp.sec.to_s) + format('%05d', timestamp.usec.to_s[0,4])
	#formattedTimestamp = timestamp.year.to_s[2,3] + timestamp.month.to_s + timestamp.day.to_s + timestamp.hour.to_s + timestamp.min.to_s + timestamp.sec.to_s + timestamp.usec.to_s[0,4]
	reverseTimestamp = (999999999999999 - formattedTimestamp.to_i).to_s 
	#puts "Reverse timestamp is : " + reverseTimestamp
	#puts "formattedTimestamp is: " + formattedTimestamp
	#puts "Sanity check         : " + (999999999999999 - reverseTimestamp.to_i).to_s
	# puts formattedTimestamp
	# puts reverseTimestamp
    	# puts msg
    	data = JSON.parse(msg)
    	#puts "Price: #{data['price']} and Object: #{data['object']}"

	newObjectId = data['object']
	newQty = data['qty'].to_i
	newType = data['type']
	newSide = data['side']
	# added for the API
	if data['conn'].nil?
		newConn = newObjectId
	else
		newConn = data['conn']
	end
	#sockJsConnection = data['sockJsConnection']
	if ((newQty <= 0) or (data['price'].to_f <= 0))
		#$redis2.publish('PrivateMessageOUT:' + sockJsConnection,'{"obj":"' + newObjectId.to_s + '","msgType":"E","M":"Order rejected: Price or qty have to be superior to 0"}')
		# to be added the day we execute through sockjs and retain the redisconnector in the json of the execution
	else
		newPriceS = newSide == "S" ? "-" : ""
		newPriceS = newPriceS + data['price']
		newPrice = newPriceS.to_f 
		#puts "The price is: " + newPriceS
		newUserId = data['user']
		newVenue = data['venue']	
	
    		#$redis2 = Redis.connect
    		newOrderNum = $redis2.incr(newObjectId + ':ORDERS:NUM').to_s
    		#puts "#{newOrderNum} is the new order num"
    		$redis2.zadd(newObjectId + ':ORDERS', newPrice, reverseTimestamp + 'ORD' + newOrderNum)
		$redis2.zadd(newObjectId + ':ORDERS:TIMESTAMP', formattedTimestamp, newOrderNum)
		$redis2.zadd(newObjectId + ':ORDERS:BYUSER:' + newUserId, formattedTimestamp, newOrderNum)
		$redis2.hmset(newObjectId + ':ORDER:DETAILS:' + newOrderNum, 'qty', newQty, 'qtyfilled', 0, 'userid', newUserId, 'price', newPrice)
		$redis2.sadd('OBJECTSTRADEDBYUSER:' + newUserId, newObjectId)

		$redis2.publish('OrderFeed:' + newObjectId, '{"obj":"' + newObjectId +'","S":"' + newSide.to_s + '","msgType":"O","V":"' + newQty.to_s + '","P":"' + newPrice.to_s + '","U":"' + newUserId.to_s + '","ON":"' + newOrderNum.to_s + '","T":"' + formattedTimestamp.to_s + '"}')
		allFillsJson = match_and_adjust_depth(newOrderNum, newSide, newQty, newPrice, newObjectId, reverseTimestamp, newUserId)
		$redis2.publish('PrivateMessageOUT:' + newConn, allFillsJson)
		#puts "Sending " + allFillsJson + " to " + newConn
	end
  else
	#puts "===== Received gibberish ===="
	#puts pattern + ' ' + event + ' ' + message


end

end

  on.punsubscribe do |event, total|
	Rails.logger.info("Unsubscribed for ##{event} (#{total} subscriptions)")
  end


module Daemons
  class Application
    def logfile;        '/home/bitnami/redis-rb/log/f1'; end
    def output_logfile; '/home/bitnami/redis-rb/log/f2'; end
  end
end

Daemons.run('sub.rb', :dir => '/home/bitnami/redis-rb/tmp/pids', :dir_mode => :normal, :ontop => false, :log_output => true)
#publish_depth(12,12,12)

end
