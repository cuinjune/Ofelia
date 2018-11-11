
touches = {} -- table as array
for i=1,10 do -- populate table, 10 should be enough ...
	touches[i] = nil
end

----------------------------------------------------
function setup()
	of.background(255)
	of.enableSmoothing()
	of.enableAlphaBlending()
end

----------------------------------------------------
function update()
end

----------------------------------------------------
function draw()
	-- draw all the current touch events
	for i,touch in ipairs(touches) do
		if touch ~= nil then
			of.pushMatrix()
				of.translate(touch.x, touch.y, 0)
							
				-- circle
				of.setColor(
					of.map(touch.x, 0, of.getWidth(), 50, 255),
					of.map(touch.id, 0, 10, 127, 255), 
					of.map(touch.y, 0, of.getHeight(), 50, 255),
					200)
				of.fill()
				of.drawCircle(0, 0, 100)

				-- id text
				of.setColor(100)
				of.drawBitmapString(tostring(touch.id), 0, -80)

			of.popMatrix()
		end
	end
end

----------------------------------------------------
function touchDown(touch)
	-- the = operator only sets table/object pointers in lua, it does not copy
	touches[touch.id+1] = touch--of.TouchEventArgs(touch) -- create new Touch & copy data
end

----------------------------------------------------
function touchMoved(touch)
	touches[touch.id+1] = touch--of.TouchEventArgs(touch)
end

----------------------------------------------------
function touchUp(touch)
	touches[touch.id+1] = nil -- setting to nil deallocates
end

----------------------------------------------------
function touchDoubleTap(touch)
	print("touchDoubleTap")
end

----------------------------------------------------
function touchCancelled(touch)
	touches[touch.id+1] = nil
end

