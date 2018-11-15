local window = pd.OFWindow()
local canvas = pd.Canvas(this)
local clock = pd.Clock(this, "setup")
local fontDir = canvas:getDir() .. "/data/"
local vidGrabber = of.VideoGrabber()
local camWidth, camHeight = 640, 480
local asciiCharacters = ""
local font = of.TrueTypeFont()

function ofelia.new()
  pd.OFWindow.addListener("setup", this)
  pd.OFWindow.addListener("update", this)
  pd.OFWindow.addListener("draw", this)
  pd.OFWindow.addListener("keyPressed", this)
  pd.OFWindow.addListener("exit", this)
  window:setPosition(30, 100)
  window:setSize(640, 480)
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
  pd.OFWindow.removeListener("exit", this)
end

function ofelia.setup()
  of.setWindowTitle("ascii video example")
  of.background(0, 0, 0)
  vidGrabber:setup(camWidth, camHeight)
  font:load(fontDir .. "Courier New Bold.ttf", 9)
  asciiCharacters = "  ..,,,'''``--_::^^**" .. '""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q'
  of.enableAlphaBlending()
end

function ofelia.update()
  vidGrabber:update()
end

function ofelia.draw()
  local videoAlphaValue = of.map(of.getMouseX(), 0, of.getWidth(), 0, 255)
  of.setColor(255, 255, 255, videoAlphaValue)
  vidGrabber:draw(0,0)
  local pixelsRef = vidGrabber:getPixels()
  of.setHexColor(0xffffff)
  for i = 0, camWidth-1, 7 do
    for j = 0, camHeight-1, 9 do
      local lightness = pixelsRef:getColor(i, j):getLightness()
      local character = math.floor(of.map(lightness, 0, 255, 0, 1)^2.5 * string.len(asciiCharacters))
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