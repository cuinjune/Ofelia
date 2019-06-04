if type(window) ~= "userdata" then
  window = ofWindow()
end

local canvas = pdCanvas(this)
local clock = pdClock(this, "setup")
local fontDir = canvas:getDir() .. "/data/"
local vidGrabber = ofVideoGrabber()
local camWidth, camHeight = 640, 480
local asciiCharacters = ""
local font = ofTrueTypeFont()

function ofelia.new()
  ofWindow.addListener("setup", this)
  ofWindow.addListener("update", this)
  ofWindow.addListener("draw", this)
  ofWindow.addListener("keyPressed", this)
  ofWindow.addListener("exit", this)
  window:setPosition(30, 100)
  window:setSize(640, 480)
  window:create()
  if ofWindow.exists then
    clock:delay(0)
  end
end

function ofelia.free()
  window:destroy()
  ofWindow.removeListener("setup", this)
  ofWindow.removeListener("update", this)
  ofWindow.removeListener("draw", this)
  ofWindow.removeListener("keyPressed", this)
  ofWindow.removeListener("exit", this)
end

function ofelia.setup()
  ofSetWindowTitle("ascii video example")
  ofBackground(0, 0, 0)
  vidGrabber:setup(camWidth, camHeight)
  font:load(fontDir .. "Courier New Bold.ttf", 9)
  asciiCharacters = "  ..,,,'''``--_::^^**" .. '""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q'
  ofEnableAlphaBlending()
end

function ofelia.update()
  vidGrabber:update()
end

function ofelia.draw()
  local videoAlphaValue = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 255)
  ofSetColor(255, 255, 255, videoAlphaValue)
  vidGrabber:draw(0,0)
  local pixelsRef = vidGrabber:getPixels()
  ofSetHexColor(0xffffff)
  for i = 0, camWidth-1, 7 do
    for j = 0, camHeight-1, 9 do
      local lightness = pixelsRef:getColor(i, j):getLightness()
      local character = math.floor(ofMap(lightness, 0, 255, 0, 1)^2.5 * string.len(asciiCharacters))
      font:drawString(string.sub(asciiCharacters, character, character+1), i, j)
    end
  end
end

function ofelia.keyPressed(e)
  if e.key == string.byte("s") or e.key == string.byte("S") then
    vidGrabber:videoSettings()
  end
end

function ofelia.exit()
  vidGrabber:close()
end