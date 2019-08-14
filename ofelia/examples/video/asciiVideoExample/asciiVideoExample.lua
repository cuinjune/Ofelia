if type(window) ~= "userdata" then
  window = ofWindow()
end

local canvas = ofCanvas(this)
local clock = ofClock(this, "setup")
local fontDir = canvas:getDir() .. "/data/"
local vidGrabber = ofVideoGrabber()
local camWidth, camHeight = 640, 480
local asciiCharacters = ""
local font = ofTrueTypeFont()

function M.new()
  ofWindow.addListener("setup", this)
  ofWindow.addListener("update", this)
  ofWindow.addListener("draw", this)
  ofWindow.addListener("keyPressed", this)
  ofWindow.addListener("exit", this)
  window:setPosition(30, 100)
  window:setSize(640, 480)
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
  ofWindow.removeListener("exit", this)
end

function M.setup()
  ofSetWindowTitle("ascii video example")
  ofSetFrameRate(10)
  ofBackground(0, 0, 0)
  vidGrabber:setup(camWidth, camHeight)
  font:load(fontDir .. "Courier New Bold.ttf", 9)
  asciiCharacters = "  ..,,,'''``--_::^^**" .. '""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q'
  ofEnableAlphaBlending()
end

function M.update()
  vidGrabber:update()
end

function M.draw()
  local videoAlphaValue = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 255)
  ofSetColor(255, 255, 255, videoAlphaValue)
  vidGrabber:draw(0,0)
  ofSetHexColor(0xffffff)
  for i = 0, camWidth-1, 7 do
    for j = 0, camHeight-1, 9 do
      local lightness = vidGrabber:getPixels():getColor(i, j):getLightness()
      local character = math.floor(ofMap(lightness, 0, 255, 0, 1)^2.5 * string.len(asciiCharacters))
      font:drawString(string.sub(asciiCharacters, character, character+1), i, j)
    end
  end
end

function M.keyPressed(e)
  if e.key == string.byte("s") or e.key == string.byte("S") then
    vidGrabber:videoSettings()
  end
end

function M.exit()
  vidGrabber:close()
end