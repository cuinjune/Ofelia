local window = pd.OFWindow()
local canvas = pd.Canvas(this)
local clock = pd.Clock(this, "setup")
local videoDir = canvas:getDir() .. "/data/"
local fingerMovie = of.VideoPlayer()
local frameByframe = false

function ofelia.new()
  pd.OFWindow.addListener("setup", this)
  pd.OFWindow.addListener("update", this)
  pd.OFWindow.addListener("draw", this)
  pd.OFWindow.addListener("keyPressed", this)
  pd.OFWindow.addListener("mouseMoved", this)
  pd.OFWindow.addListener("mouseDragged", this)
  pd.OFWindow.addListener("mousePressed", this)
  pd.OFWindow.addListener("mouseReleased", this)
  pd.OFWindow.addListener("exit", this)
  window:setPosition(30, 100)
  window:setSize(1024, 768)
  window:create()
  if pd.OFWindow.exists then
    clock:delay(0)
  end
end

function ofelia.free()
  window:destroy()
  pd.OFWindow.removeListener("setup", this)
  pd.OFWindow.removeListener("update", this)
  pd.OFWindow.removeListener("draw", this)
  pd.OFWindow.removeListener("keyPressed", this)
  pd.OFWindow.removeListener("mouseMoved", this)
  pd.OFWindow.removeListener("mouseDragged", this)
  pd.OFWindow.removeListener("mousePressed", this)
  pd.OFWindow.removeListener("mouseReleased", this)
  pd.OFWindow.removeListener("exit", this)
end

function ofelia.setup()
  of.setWindowTitle("video player example")
  of.background(255, 255, 255, 255)
  frameByframe = false
  fingerMovie:load(videoDir .. "fingers.mov")
  fingerMovie:setLoopState(of.LOOP_NORMAL)
  fingerMovie:play()
end

function ofelia.update()
  fingerMovie:update()
end

function ofelia.draw()
  of.setHexColor(0xFFFFFF)
  fingerMovie:draw(20, 20)
  of.setHexColor(0x000000)
  local pixels = fingerMovie:getPixels()
  local vidWidth = pixels:getWidth()
  local vidHeight = pixels:getHeight()
  local nChannels = pixels:getNumChannels()
  for i = 4, vidWidth-1, 8 do
     for j = 4, vidHeight-1, 8 do
       local r = pixels:getColor((j * 320 + i)*nChannels).r
       local val = 1 - (r / 255)
       of.drawCircle(400 + i, 20+j, 10*val)
     end
  end
  of.setHexColor(0x000000)
  of.drawBitmapString("press f to change", 20, 320)
  if frameByframe then
    of.setHexColor(0xCCCCCC)
  end
  of.drawBitmapString("mouse speed position", 20, 340)
  if not frameByframe then
    of.setHexColor(0xCCCCCC)
  else
    of.setHexColor(0x000000)
  end
  of.drawBitmapString("keys <- -> frame by frame ", 190, 340)
  of.setHexColor(0x000000)
  of.drawBitmapString(string.format("frame: %d / %d", fingerMovie:getCurrentFrame(), fingerMovie:getTotalNumFrames()), 20, 380)
  of.drawBitmapString(string.format("duration: %.2f / %.2f", fingerMovie:getPosition()*fingerMovie:getDuration(), fingerMovie:getDuration()), 20, 400)
  of.drawBitmapString(string.format("speed: %.2f", fingerMovie:getSpeed()), 20, 420)
  if fingerMovie:getIsMovieDone() then
    of.setHexColor(0xFF0000)
    of.drawBitmapString("end of movie", 20, 440)
  end
end

function ofelia.keyPressed(e)
  if e.key == string.byte("f") then
    frameByframe = frameByframe == false
    fingerMovie:setPaused(frameByframe)
  elseif e.key == of.KEY_LEFT then
    fingerMovie:previousFrame()
  elseif e.key == of.KEY_RIGHT then
    fingerMovie:nextFrame()
  elseif e.key == string.byte("0") then
    fingerMovie:firstFrame()
  end
end

function ofelia.mouseMoved(e)
  if not frameByframe then
    local width = of.getWidth()
    local pct = e.x / width
    local speed = (2 * pct - 1) * 5
    fingerMovie:setSpeed(speed)
  end
end

function ofelia.mouseDragged(e)
  if not frameByframe then
    local width = of.getWidth()
    local pct = e.x / width
    fingerMovie:setPosition(pct)
  end
end

function ofelia.mousePressed(e)
  if not frameByframe then
    fingerMovie:setPaused(true)
  end
end

function ofelia.mouseReleased(e)
  if not frameByframe then
    fingerMovie:setPaused(false)
  end
end

function ofelia.exit()
  fingerMovie:close()
end