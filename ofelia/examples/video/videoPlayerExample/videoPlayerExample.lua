if type(window) ~= "userdata" then
  window = ofWindow()
end

local canvas = ofCanvas(this)
local clock = ofClock(this, "setup")
local videoDir = canvas:getDir() .. "/data/"
local fingerMovie = ofVideoPlayer()
local frameByframe = false

function M.new()
  ofWindow.addListener("setup", this)
  ofWindow.addListener("update", this)
  ofWindow.addListener("draw", this)
  ofWindow.addListener("keyPressed", this)
  ofWindow.addListener("mouseMoved", this)
  ofWindow.addListener("mouseDragged", this)
  ofWindow.addListener("mousePressed", this)
  ofWindow.addListener("mouseReleased", this)
  ofWindow.addListener("exit", this)
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
  ofWindow.removeListener("mouseMoved", this)
  ofWindow.removeListener("mouseDragged", this)
  ofWindow.removeListener("mousePressed", this)
  ofWindow.removeListener("mouseReleased", this)
  ofWindow.removeListener("exit", this)
end

function M.setup()
  ofSetWindowTitle("video player example")
  ofBackground(255, 255, 255, 255)
  frameByframe = false
  fingerMovie:load(videoDir .. "fingers.mov")
  fingerMovie:setLoopState(OF_LOOP_NORMAL)
  fingerMovie:play()
end

function M.update()
  fingerMovie:update()
end

function M.draw()
  ofSetHexColor(0xFFFFFF)
  fingerMovie:draw(20, 20)
  ofSetHexColor(0x000000)
  local pixels = fingerMovie:getPixels()
  local vidWidth = pixels:getWidth()
  local vidHeight = pixels:getHeight()
  local nChannels = pixels:getNumChannels()
  for i = 4, vidWidth-1, 8 do
     for j = 4, vidHeight-1, 8 do
       local r = pixels:getColor((j * 320 + i)*nChannels).r
       local val = 1 - (r / 255)
       ofDrawCircle(400 + i, 20+j, 10*val)
     end
  end
  ofSetHexColor(0x000000)
  ofDrawBitmapString("press f to change", 20, 320)
  if frameByframe then
    ofSetHexColor(0xCCCCCC)
  end
  ofDrawBitmapString("mouse speed position", 20, 340)
  if not frameByframe then
    ofSetHexColor(0xCCCCCC)
  else
    ofSetHexColor(0x000000)
  end
  ofDrawBitmapString("keys <- -> frame by frame ", 190, 340)
  ofSetHexColor(0x000000)
  ofDrawBitmapString(string.format("frame: %d / %d", fingerMovie:getCurrentFrame(), fingerMovie:getTotalNumFrames()), 20, 380)
  ofDrawBitmapString(string.format("duration: %.2f / %.2f", fingerMovie:getPosition()*fingerMovie:getDuration(), fingerMovie:getDuration()), 20, 400)
  ofDrawBitmapString(string.format("speed: %.2f", fingerMovie:getSpeed()), 20, 420)
  if fingerMovie:getIsMovieDone() then
    ofSetHexColor(0xFF0000)
    ofDrawBitmapString("end of movie", 20, 440)
  end
end

function M.keyPressed(e)
  if e.key == string.byte("f") then
    frameByframe = frameByframe == false
    fingerMovie:setPaused(frameByframe)
  elseif e.key == OF_KEY_LEFT then
    fingerMovie:previousFrame()
  elseif e.key == OF_KEY_RIGHT then
    fingerMovie:nextFrame()
  elseif e.key == string.byte("0") then
    fingerMovie:firstFrame()
  end
end

function M.mouseMoved(e)
  if not frameByframe then
    local width = ofGetWidth()
    local pct = e.x / width
    local speed = (2 * pct - 1) * 5
    fingerMovie:setSpeed(speed)
  end
end

function M.mouseDragged(e)
  if not frameByframe then
    local width = ofGetWidth()
    local pct = e.x / width
    fingerMovie:setPosition(pct)
  end
end

function M.mousePressed(e)
  if not frameByframe then
    fingerMovie:setPaused(true)
  end
end

function M.mouseReleased(e)
  if not frameByframe then
    fingerMovie:setPaused(false)
  end
end

function M.exit()
  fingerMovie:close()
end