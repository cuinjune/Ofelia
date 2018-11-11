
-- simple class, like a C struct

DraggableVertex = class()

function DraggableVertex:__init(x, y)
	self.x = x
	self.y = y
	self.bBeingDragged = false
	self.bOver = false
	self.radius = 4
end

-- array of DraggableVertex objects using a lua table
curveVertices = {
	DraggableVertex(326, 209),
	DraggableVertex(306, 279),
	DraggableVertex(265, 331),
	DraggableVertex(304, 383),
	DraggableVertex(374, 383),
	DraggableVertex(418, 209),
	DraggableVertex(345, 279)
}

-- number of vertexes in the array
nCurveVertexes = 7

----------------------------------------------------
function setup()
	of.setWindowTitle("polygon example")
end

----------------------------------------------------
function update()
	of.background(255, 255, 255)
	of.setFrameRate(60)
end

----------------------------------------------------
function draw()
	of.fill()
	of.setHexColor(0xe0be21)

	--------(a)--------------------------------------
	-- 
	-- 		draw a star
	--
	-- 		use poly winding odd, the default rule
	--
	-- 		info about the winding rules is here:
	--		http:--glprogramming.com/red/images/Image128.gif
	-- 
	of.setPolyMode(of.POLY_WINDING_ODD) -- this is the normal mode
	of.beginShape()
		of.vertex(200, 135)
		of.vertex(15, 135)
		of.vertex(165, 25)
		of.vertex(105, 200)
		of.vertex(50, 25)
	of.endShape(false)

	--------(b)--------------------------------------
	-- 
	-- 		draw a star
	--
	-- 		use poly winding nonzero
	--
	-- 		info about the winding rules is here:
	--		http:--glprogramming.com/red/images/Image128.gif
	-- 
	of.setHexColor(0xb5de10)
	of.setPolyMode(of.POLY_WINDING_NONZERO)
	of.beginShape()
		of.vertex(400, 135)
		of.vertex(215, 135)
		of.vertex(365, 25)
		of.vertex(305, 200)
		of.vertex(250, 25)
	of.endShape(false)
	---------------------------------------

	--------(c)--------------------------------------
	-- 
	-- 		draw a star dynamically
	--
	-- 		use the mouse position as a pct
	--		to calc nPoints and internal point radius
	--
	local xPct = of.getMouseX() / of.getWidth()
	local yPct = of.getMouseY() / of.getHeight()
	local nTips = 5 + xPct * 60
	local nStarPts = nTips * 2
	local angleChangePerPt = of.TWO_PI / nStarPts
	local innerRadius = 0 + yPct*80
	local outerRadius = 80
	local origx = 525
	local origy = 100
	local angle = 0

	of.setHexColor(0xa16bca)
	of.beginShape()
	for i=0,nStarPts do
		if i % 2 == 0 then
			-- inside point:
			local x = origx + innerRadius * math.cos(angle)
			local y = origy + innerRadius * math.sin(angle)
			of.vertex(x, y)
		else
			-- outside point
			local x = origx + outerRadius * math.cos(angle)
			local y = origy + outerRadius * math.sin(angle)
			of.vertex(x, y)
		end
		angle = angle + angleChangePerPt
	end
	of.endShape(false)
	---------------------------------------

	--------(d)--------------------------------------
	-- 
	-- 		poylgon of random points
	--
	-- 		lots of self intersection, 500 pts is a good stress test
	-- 
	-- 
	of.setHexColor(0x0cb0b6)
	of.setPolyMode(of.POLY_WINDING_ODD)
	of.beginShape()
	for i=1,10 do
		of.vertex(of.random(650, 850), of.random(20, 200))
	end
	of.endShape(false)
	---------------------------------------

	--------(e)--------------------------------------
	-- 
	-- 		use sin cos and time to make some spirally shape
	--
	of.pushMatrix()
		of.translate(100, 300, 0)
		of.setHexColor(0xff2220)
		of.fill()
		of.setPolyMode(of.POLY_WINDING_ODD)
		of.beginShape()
		local angleStep 	= of.TWO_PI/(100 + math.sin(of.getElapsedTimef()/5) * 60)
		local radiusAdder 	= 0.5
		local radius 		= 0
		for i=1,200 do
			local anglef = (i) * angleStep
			local x = radius * math.cos(anglef)
			local y = radius * math.sin(anglef) 
			of.vertex(x, y)
			radius 	= radius + radiusAdder
		end
		of.endShape(of.CLOSE)
	of.popMatrix()
	---------------------------------------

	--------(f)--------------------------------------
	-- 
	-- 		ofCurveVertex
	-- 
	-- 		because it uses catmul rom splines, we need to repeat the first and last 
	-- 		items so the curve actually goes through those points
	--

	of.setHexColor(0x2bdbe6)
	of.beginShape()
	
		for i=1,nCurveVertexes do
			
			-- sorry about all the if/states here, but to do catmull rom curves
			-- we need to duplicate the start and end points so the curve acutally 
			-- goes through them.
			
			-- for i == 0, we just call the vertex twice
			-- for i == nCurveVertexes-1 (last point) we call vertex 0 twice
			-- otherwise just normal ofCurveVertex call
			
			if i == 1 then
				-- we need to duplicate 0 for the curve to start at point 0
				of.curveVertex(curveVertices[1].x, curveVertices[1].y)
				-- we need to duplicate 0 for the curve to start at point 0
				of.curveVertex(curveVertices[1].x, curveVertices[1].y) 
			elseif i == nCurveVertexes then
				of.curveVertex(curveVertices[i].x, curveVertices[i].y)
				-- to draw a curve from pt 6 to pt 0
				of.curveVertex(curveVertices[1].x, curveVertices[1].y)
				-- we duplicate the first point twice
				of.curveVertex(curveVertices[1].x, curveVertices[1].y)
			else
				of.curveVertex(curveVertices[i].x, curveVertices[i].y)
			end
		end
		
	of.endShape(false)
	
	
	-- show a faint the non-curve version of the same polygon:
	of.enableAlphaBlending()
		of.noFill()
		of.setColor(0, 0, 0, 40)
		of.beginShape()
			for i=1,nCurveVertexes do
				of.vertex(curveVertices[i].x, curveVertices[i].y)
			end
		of.endShape(true)
		
		
		of.setColor(0, 0, 0, 80)
		for i=1,nCurveVertexes do
			if curveVertices[i].bOver == true then 
				of.fill()
			else 
				of.noFill()
			end
			of.drawCircle(curveVertices[i].x, curveVertices[i].y, 4)
		end
	of.disableAlphaBlending()
	---------------------------------------

	--------(g)--------------------------------------
	-- 
	-- 		ofBezierVertex
	-- 
	-- 		with ofBezierVertex we can draw a curve from the current vertex
	--		through the the next three vertexes we pass in.
	--		(two control points and the final bezier point)
	--		
	
	local x0 = 500
	local y0 = 300
	local x1 = 550+50*math.cos(of.getElapsedTimef()*1.0)
	local y1 = 300+100*math.sin(of.getElapsedTimef()/3.5)
	local x2 = 600+30*math.cos(of.getElapsedTimef()*2.0)
	local y2 = 300+100*math.sin(of.getElapsedTimef())
	local x3 = 650
	local y3 = 300
	
	of.fill()
	of.setHexColor(0xFF9933)
	of.beginShape()
	of.vertex(x0, y0)
	of.bezierVertex(x1, y1, x2, y2, x3, y3)
	of.endShape(false)
	
	of.enableAlphaBlending()
		of.fill()
		of.setColor(0, 0 ,0, 40)
		of.drawCircle(x0, y0, 4)
		of.drawCircle(x1, y1, 4)
		of.drawCircle(x2, y2, 4)
		of.drawCircle(x3, y3, 4)
	of.disableAlphaBlending()
	---------------------------------------
	
	--------(h)--------------------------------------
	-- 
	-- 		holes / ofNextContour
	-- 
	-- 		with ofNextContour we can create multi-contour shapes
	-- 		this allows us to draw holes, for example... 
	--
	of.fill()
	of.setHexColor(0xd3ffd3)
	of.drawRectangle(80,480, 140, 70)
	of.setHexColor(0xff00ff)
	
	of.beginShape()
		
		of.vertex(100, 500)
		of.vertex(180, 550)
		of.vertex(100, 600)
		
		of.nextContour(true)
		
		of.vertex(120, 520)
		of.vertex(160, 550)
		of.vertex(120, 580)
		
	of.endShape(true)
	---------------------------------------

	--------(i)--------------------------------------
	-- 
	-- 		CSG / ofNextContour
	-- 
	-- 		with different winding rules, you can even use ofNextContour to 
	-- 		perform constructive solid geometry 
	-- 		
	-- 		be careful, the clockwiseness or counter clockwisenss of your multiple
	-- 		contours matters with these winding rules.
	--
	-- 		for csg ideas, see : http:--glprogramming.com/red/chapter11.html
	-- 
	-- 		info about the winding rules is here:
	--		http:--glprogramming.com/red/images/Image128.gif
	-- 
	of.noFill()
	
	of.pushMatrix()
	
	of.setPolyMode(of.POLY_WINDING_ODD)
	
	of.beginShape()
		
		of.vertex(300, 500)
		of.vertex(380, 550)
		of.vertex(300, 600)
		
		of.nextContour(true)
		
		for i=1,20 do
			local anglef = (i / 19.0) * of.TWO_PI
			local x = 340 + 30 * math.cos(anglef)
			local y = 550 + 30 * math.sin(anglef) 
			of.vertex(x,y)
			radius 	= radius + radiusAdder 
		end
		
	of.endShape(true)
	
	of.translate(100, 0, 0)
	
	of.setPolyMode(of.POLY_WINDING_NONZERO)	
	of.beginShape()
		
		of.vertex(300, 500)
		of.vertex(380, 550)
		of.vertex(300, 600)
		
		of.nextContour(true)
		
		for i=1,20 do
			local anglef = (i / 19.0) * of.TWO_PI
			local x = 340 + 30 * math.cos(anglef)
			local y = 550 + 30 * math.sin(anglef) 
			of.vertex(x,y)
			radius 	= radius + radiusAdder 
		end
		
	of.endShape(true)
	
	of.translate(100, 0, 0)
	of.setPolyMode(of.POLY_WINDING_ABS_GEQ_TWO)
	of.beginShape()
		of.vertex(300, 500)
		of.vertex(380, 550)
		of.vertex(300, 600)
		of.nextContour(true)
		
		for i=1,20 do
			local anglef = (i / 19.0) * of.TWO_PI
			local x = 340 + 30 * math.cos(anglef)
			local y = 550 + 30 * math.sin(anglef) 
			of.vertex(x,y)
			radius 	= radius + radiusAdder 
		end
				
	of.endShape(true)
	
	of.popMatrix()
	---------------------------------------
	
	of.setHexColor(0x000000)
	of.drawBitmapString("(a) star\nwinding rule odd", 20, 210)
	
	of.setHexColor(0x000000)
	of.drawBitmapString("(b) star\nwinding rule nonzero", 220, 210)
	
	of.setHexColor(0x000000)
	of.drawBitmapString("(c) dynamically\ncreated shape", 420, 210)
	
	of.setHexColor(0x000000)
	of.drawBitmapString("(d) random points\npoly", 670, 210)
	
	of.setHexColor(0x000000)
	of.drawBitmapString("(e) fun with sin/cos", 20, 410)
	
	of.setHexColor(0x000000)
	of.drawBitmapString("(f) ofCurveVertex\nuses catmull rom\nto make curved shapes", 220, 410)
	
	of.setHexColor(0x000000)
	of.drawBitmapString("(g) ofBezierVertex\nuses bezier to draw curves", 460, 410)
	
	
	of.setHexColor(0x000000)
	of.drawBitmapString("(h) ofNextContour\nallows for holes", 20, 610)
	
	of.setHexColor(0x000000)
	of.drawBitmapString("(i) ofNextContour\ncan even be used for CSG operations\nsuch as union and intersection", 260, 620)
end

function exit()
	print("script finished")
end

-- input callbacks

----------------------------------------------------
function mouseMoved(x, y)
	for i=1,nCurveVertexes do
		local diffx = x - curveVertices[i].x
		local diffy = y - curveVertices[i].y
		local dist = math.sqrt(diffx*diffx + diffy*diffy)
		if dist < curveVertices[i].radius then
			curveVertices[i].bOver = true
		else
			curveVertices[i].bOver = false
		end
	end
end

----------------------------------------------------
function mouseDragged(x, y, button)
	for i=1,nCurveVertexes do
		if curveVertices[i].bBeingDragged == true then
			curveVertices[i].x = x
			curveVertices[i].y = y
		end
	end
end

----------------------------------------------------
function mousePressed(x, y, button)
	for i=1,nCurveVertexes do
		local diffx = x - curveVertices[i].x
		local diffy = y - curveVertices[i].y
		local dist = math.sqrt(diffx*diffx + diffy*diffy)
		if dist < curveVertices[i].radius then
			curveVertices[i].bBeingDragged = true
		else
			curveVertices[i].bBeingDragged = false
		end	
	end
end

----------------------------------------------------
function mouseReleased(x, y, button)
	for i=1,nCurveVertexes do
		curveVertices[i].bBeingDragged = false	
	end
end


