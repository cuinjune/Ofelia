if type(window) ~= "userdata" then
  window = ofWindow()
end

local clock = ofClock(this, "setup")
local DV = class()
function DV:__init(x, y)
  self.x = x
  self.y = y
  self.bBeingDragged = false
  self.bOver = false
  self.radius = 4
end
local nCurveVertexes = 0
local curveVertices = {}

function M.new()
  ofWindow.addListener("setup", this)
  ofWindow.addListener("draw", this)
  ofWindow.addListener("mouseMoved", this)
  ofWindow.addListener("mouseDragged", this)
  ofWindow.addListener("mousePressed", this)
  ofWindow.addListener("mouseReleased", this)
  window:setPosition(30, 100)
  window:setSize(1024, 768)
  if ofWindow.exists then
    clock:delay(0)
  else
    window:create()
  end
end

function M.free()
  window:destroy()
  ofWindow.removeListener("setup", this)
  ofWindow.removeListener("draw", this)
  ofWindow.removeListener("mouseMoved", this)
  ofWindow.removeListener("mouseDragged", this)
  ofWindow.removeListener("mousePressed", this)
  ofWindow.removeListener("mouseReleased", this)
end

function M.setup()
  ofSetWindowTitle("polygon example")
  ofBackground(255, 255, 255, 255)
  nCurveVertexes = 7
  curveVertices = {DV(326, 209), DV(306, 279), DV(265, 331), DV(304, 383), DV(374, 383), DV(418, 209), DV(345, 279)}
end

function M.draw()
  ofFill()

  --[[---------------------------------------------------------------
      draw a star
      use poly winding odd, the default rule
  --]]---------------------------------------------------------------
  ofSetHexColor(0xe0be21)
  ofSetPolyMode(OF_POLY_WINDING_ODD)
  ofBeginShape()
    ofVertex(200, 135)
    ofVertex(15, 135)
    ofVertex(165, 25)
    ofVertex(105, 200)
    ofVertex(50, 25)
  ofEndShape()

  --[[---------------------------------------------------------------
      draw a star
      use poly winding nonzero
  --]]---------------------------------------------------------------
  ofSetHexColor(0xb5de10)
  ofSetPolyMode(OF_POLY_WINDING_NONZERO)
  ofBeginShape()
    ofVertex(400, 135)
    ofVertex(215, 135)
    ofVertex(365, 25)
    ofVertex(305, 200)
    ofVertex(250, 25)
  ofEndShape()

  --[[---------------------------------------------------------------
      draw a star dynamically
      use the mouse position as a pct
      to calc nPoints and internal point radius
  --]]---------------------------------------------------------------
  local xPct = ofGetMouseX() / ofGetWidth()
  local yPct = ofGetMouseY() / ofGetHeight()
  local nTips = 5 + xPct * 60
  local nStarPts = math.min(nTips * 2, 100)
  local angleChangePerPt = OF_TWO_PI / nStarPts
  local innerRadius = 0 + yPct*80
  local outerRadius = 80
  local origx = 525
  local origy = 100
  local angle = 0
  ofSetHexColor(0xa16bca)
  ofBeginShape()
  for i=0,nStarPts do
    if i % 2 == 0 then
      local x = origx + innerRadius * math.cos(angle)
      local y = origy + innerRadius * math.sin(angle)
      ofVertex(x, y)
    else
      local x = origx + outerRadius * math.cos(angle)
      local y = origy + outerRadius * math.sin(angle)
      ofVertex(x, y)
    end
    angle = angle + angleChangePerPt
  end
  ofEndShape()

  --[[---------------------------------------------------------------
      poylgon of random points
      lots of self intersection, 500 pts is a good stress test
  --]]---------------------------------------------------------------
  ofSetHexColor(0x0cb0b6)
  ofSetPolyMode(OF_POLY_WINDING_ODD)
  ofBeginShape()
  for i=1,10 do
    ofVertex(ofRandom(650, 850), ofRandom(20, 200))
  end
  ofEndShape()

  --[[---------------------------------------------------------------
      use sin cos and time to make some spirally shape
  --]]---------------------------------------------------------------
  ofPushMatrix()
    ofTranslate(100, 300, 0)
    ofSetHexColor(0xff2220)
    ofFill()
    ofSetPolyMode(OF_POLY_WINDING_ODD)
    ofBeginShape()
    local angleStep = OF_TWO_PI/(100 + math.sin(ofGetElapsedTimef()/5) * 60)
    local radiusAdder = 0.5
    local radius = 0
    for i=1,200 do
      local anglef = (i) * angleStep
      local x = radius * math.cos(anglef)
      local y = radius * math.sin(anglef) 
      ofVertex(x, y)
      radius = radius + radiusAdder
    end
    ofEndShape(OF_CLOSE)
  ofPopMatrix()

  --[[---------------------------------------------------------------
      ofCurveVertex
      because it uses catmul rom splines, we need to repeat the first and last
      items so the curve actually goes through those points
  --]]---------------------------------------------------------------
  ofSetHexColor(0x2bdbe6)
  ofBeginShape()	
  for i=1,nCurveVertexes do
    if i == 1 then
      ofCurveVertex(curveVertices[1].x, curveVertices[1].y)
      ofCurveVertex(curveVertices[1].x, curveVertices[1].y) 
    elseif i == nCurveVertexes then
      ofCurveVertex(curveVertices[i].x, curveVertices[i].y)
      ofCurveVertex(curveVertices[1].x, curveVertices[1].y)
      ofCurveVertex(curveVertices[1].x, curveVertices[1].y)
    else
      ofCurveVertex(curveVertices[i].x, curveVertices[i].y)
    end
  end	
  ofEndShape()
  ofEnableAlphaBlending()
    ofNoFill()
    ofSetColor(0, 0, 0, 40)
    ofBeginShape()
    for i=1,nCurveVertexes do
      ofVertex(curveVertices[i].x, curveVertices[i].y)
    end
    ofEndShape(true)
    ofSetColor(0, 0, 0, 80)
    for i=1,nCurveVertexes do
      if curveVertices[i].bOver == true then 
        ofFill()
      else 
        ofNoFill()
      end
      ofDrawCircle(curveVertices[i].x, curveVertices[i].y, 4)
    end
  ofDisableAlphaBlending()

  --[[---------------------------------------------------------------
      ofBezierVertex
      with ofBezierVertex we can draw a curve from the current vertex
      through the the next three vertexes we pass in.
      (two control points and the final bezier point)
  --]]---------------------------------------------------------------
  local x0 = 500
  local y0 = 300
  local x1 = 550+50*math.cos(ofGetElapsedTimef()*1.0)
  local y1 = 300+100*math.sin(ofGetElapsedTimef()/3.5)
  local x2 = 600+30*math.cos(ofGetElapsedTimef()*2.0)
  local y2 = 300+100*math.sin(ofGetElapsedTimef())
  local x3 = 650
  local y3 = 300
  ofFill()
  ofSetHexColor(0xFF9933)
  ofBeginShape()
  ofVertex(x0, y0)
  ofBezierVertex(x1, y1, x2, y2, x3, y3)
  ofEndShape()
  ofEnableAlphaBlending()
    ofFill()
    ofSetColor(0, 0 ,0, 40)
    ofDrawCircle(x0, y0, 4)
    ofDrawCircle(x1, y1, 4)
    ofDrawCircle(x2, y2, 4)
    ofDrawCircle(x3, y3, 4)
  ofDisableAlphaBlending()

  --[[---------------------------------------------------------------
      holes / ofNextContour
      with ofNextContour we can create multi-contour shapes
      this allows us to draw holes, for example... 
  --]]---------------------------------------------------------------
  ofFill()
  ofSetHexColor(0xd3ffd3)
  ofDrawRectangle(80,480, 140, 70)
  ofSetHexColor(0xff00ff)
  ofBeginShape()
    ofVertex(100, 500)
    ofVertex(180, 550)
    ofVertex(100, 600)	
    ofNextContour(true)
    ofVertex(120, 520)
    ofVertex(160, 550)
    ofVertex(120, 580)
  ofEndShape(true)

  --[[---------------------------------------------------------------
      CSG / ofNextContour
      with different winding rules, you can even use ofNextContour to
      perform constructive solid geometry
  --]]---------------------------------------------------------------
  ofNoFill()	
  ofPushMatrix()	
  ofSetPolyMode(OF_POLY_WINDING_ODD)	
  ofBeginShape()		
    ofVertex(300, 500)
    ofVertex(380, 550)
    ofVertex(300, 600)
    ofNextContour(true)		
    for i=1,20 do
      local anglef = (i / 19.0) * OF_TWO_PI
      local x = 340 + 30 * math.cos(anglef)
      local y = 550 + 30 * math.sin(anglef) 
      ofVertex(x,y)
      radius = radius + radiusAdder 
    end
  ofEndShape(true)
  ofTranslate(100, 0, 0)
  ofSetPolyMode(OF_POLY_WINDING_NONZERO)	
  ofBeginShape()
    ofVertex(300, 500)
    ofVertex(380, 550)
    ofVertex(300, 600)
    ofNextContour(true)
    for i=1,20 do
      local anglef = (i / 19.0) * OF_TWO_PI
      local x = 340 + 30 * math.cos(anglef)
      local y = 550 + 30 * math.sin(anglef) 
      ofVertex(x,y)
      radius = radius + radiusAdder 
    end
  ofEndShape(true)
  ofTranslate(100, 0, 0)
  ofSetPolyMode(OF_POLY_WINDING_ABS_GEQ_TWO)
  ofBeginShape()
    ofVertex(300, 500)
    ofVertex(380, 550)
    ofVertex(300, 600)
    ofNextContour(true)
    for i=1,20 do
      local anglef = (i / 19.0) * OF_TWO_PI
      local x = 340 + 30 * math.cos(anglef)
      local y = 550 + 30 * math.sin(anglef) 
      ofVertex(x,y)
      radius = radius + radiusAdder 
    end			
  ofEndShape(true)
  ofPopMatrix()
  ofSetHexColor(0x000000)
  ofDrawBitmapString("(a) star"..string.char(10).."winding rule odd", 20, 210)
  ofSetHexColor(0x000000)
  ofDrawBitmapString("(b) star"..string.char(10).."winding rule nonzero", 220, 210)
  ofSetHexColor(0x000000)
  ofDrawBitmapString("(c) dynamically"..string.char(10).."created shape", 420, 210)
  ofSetHexColor(0x000000)
  ofDrawBitmapString("(d) random points"..string.char(10).."poly", 670, 210)	
  ofSetHexColor(0x000000)
  ofDrawBitmapString("(e) fun with sin/cos", 20, 410)	
  ofSetHexColor(0x000000)
  ofDrawBitmapString("(f) ofCurveVertex"..string.char(10).."uses catmull rom"..string.char(10).."to make curved shapes", 220, 410)
  ofSetHexColor(0x000000)
  ofDrawBitmapString("(g) ofBezierVertex"..string.char(10).."uses bezier to draw curves", 460, 410)
  ofSetHexColor(0x000000)
  ofDrawBitmapString("(h) ofNextContour"..string.char(10).."allows for holes", 20, 610)
  ofSetHexColor(0x000000)
  ofDrawBitmapString("(i) ofNextContour"..string.char(10).."can even be used for CSG operations"..string.char(10).."such as union and intersection", 260, 620)
end

function M.mouseMoved(e)
  for i=1,nCurveVertexes do
    local diffx = e.x - curveVertices[i].x
    local diffy = e.y - curveVertices[i].y
    local dist = math.sqrt(diffx*diffx + diffy*diffy)
    if dist < curveVertices[i].radius then
      curveVertices[i].bOver = true
    else
      curveVertices[i].bOver = false
    end
  end
end

function M.mouseDragged(e)
  for i=1,nCurveVertexes do
    if curveVertices[i].bBeingDragged == true then
      curveVertices[i].x = e.x
      curveVertices[i].y = e.y
    end
  end
end

function M.mousePressed(e)
  for i=1,nCurveVertexes do
    local diffx = e.x - curveVertices[i].x
    local diffy = e.y - curveVertices[i].y
    local dist = math.sqrt(diffx*diffx + diffy*diffy)
    if dist < curveVertices[i].radius then
      curveVertices[i].bBeingDragged = true
    else
      curveVertices[i].bBeingDragged = false
    end	
  end
end

function M.mouseReleased(e)
  for i=1,nCurveVertexes do
    curveVertices[i].bBeingDragged = false	
  end
end