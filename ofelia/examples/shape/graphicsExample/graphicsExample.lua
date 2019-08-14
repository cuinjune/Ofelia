if type(window) ~= "userdata" then
  window = ofWindow()
end

local clock = ofClock(this, "setup")
local counter = 0
local bSmooth = false

function M.new()
  ofWindow.addListener("setup", this)
  ofWindow.addListener("update", this)
  ofWindow.addListener("draw", this)
  ofWindow.addListener("keyPressed", this)
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
  ofWindow.removeListener("update", this)
  ofWindow.removeListener("draw", this)
  ofWindow.removeListener("keyPressed", this)
end

function M.setup()
  ofSetWindowTitle("graphics example")
  ofSetCircleResolution(50)
  ofBackground(255, 255, 255, 255)
  ofDisableSmoothing()
end

function M.update()
  counter = counter + 0.033
end

function M.draw()
  ofSetColor(255, 130, 0)
  local radius = 50 + 10 * math.sin(counter)
  ofFill()
  ofDrawCircle(100, 400, radius)
  ofNoFill()
  ofSetHexColor(0xCCCCCC)
  ofDrawCircle(100, 400, 80)
  ofSetHexColor(0x000000)
  ofDrawBitmapString("circle", 75, 500)
  ofFill()
  for i=0,200 do
    ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255))
    ofDrawRectangle(ofRandom(250, 350), ofRandom(350, 450), ofRandom(10, 20), ofRandom(10, 20))
  end
  ofSetHexColor(0x000000)
  ofDrawBitmapString("rectangles", 275, 500)
  ofSetHexColor(0x00FF33)
  ofDrawRectangle(400, 350, 100, 100)
  ofEnableAlphaBlending()
  ofSetColor(255, 0, 0, 127)
  ofDrawRectangle(450, 430, 100, 33)
  ofSetColor(255, 0, 0, math.fmod(counter*10, 255))
  ofDrawRectangle(450, 370, 100, 33)
  ofDisableAlphaBlending()
  ofSetHexColor(0x000000)
  ofDrawBitmapString("transparency", 410, 500)
  if bSmooth then
    ofEnableSmoothing()
  end
  ofSetHexColor(0xFF0000)
    for i=0,20 do
      ofDrawLine(600, 300 + (i*5), 800, 250 + (i*10))
    end
  if bSmooth then
    ofDisableSmoothing()
  end
  ofSetHexColor(0x000000)
  ofDrawBitmapString("lines"..string.char(10).."press 's' to toggle smoothness", 600, 500)
end

function M.keyPressed(e)
  if e.key == string.byte("s") then
    bSmooth = not bSmooth
  end
end