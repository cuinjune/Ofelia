local window = pd.OFWindow()
local canvas = pd.Canvas(this)
local clock = pd.Clock(this, "setup")
local fontDir = canvas:getDir() .. "/data/"
local franklinBook = of.TrueTypeFont()
local verdana = of.TrueTypeFont()
local franklinBookLarge = of.TrueTypeFont()
local counter = 0

function ofelia.new()
  pd.OFWindow.addListener("setup", this)
  pd.OFWindow.addListener("update", this)
  pd.OFWindow.addListener("draw", this)
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
end

function ofelia.setup()
  of.setWindowTitle("fonts example")
  franklinBook:load(fontDir .. "frabk.ttf", 32)
  verdana:load(fontDir .. "verdana.ttf", 8, false, true)
  verdana:setLineHeight(20)
end

function ofelia.update()
  of.background(255, 255, 255)	
  counter = counter + 1
end

function ofelia.draw()
  of.setHexColor(0x00FF00)
  franklinBook:drawString("hello, this is franklin book calling"..string.char(10).."anyone home?", 100, 100)
  of.setHexColor(0x000000)
  verdana:drawString("hello, I am aliased verdana -- full character set, see: � ! ", 100, 210)
  of.setHexColor(0x00FF00)
  franklinBook:drawString("I can't make an (�) like you", 100, 310)
  of.setHexColor(0x000000)
  verdana:drawString("yeah, but I'm not exactly pretty"..string.char(10).."the problem is with freeType library..."..string.char(10).."apple has a patent on TTF font hints"..string.char(10).."so our aliased type via freeType isn't super looking", 100, 380)
  of.setHexColor(0x00FF00)
  franklinBook:drawString("you look ok ! don't worry", 100, 520)
  tempString = tostring(counter)
  rect = franklinBook:getStringBoundingBox(tempString, 0, 0)
  local centerx = rect.x + rect.width / 2
  local centery = rect.y + rect.height / 2
  of.pushMatrix()
    of.translate(100, 650, 0)
    of.rotateDeg(counter, 0, 0, 1)
    of.setHexColor(0xcccccc)
    of.drawRectangle(rect.x - centerx, rect.y - centery, rect.width, rect.height)
    of.setHexColor(0xff3399)
    franklinBook:drawString(tempString, -centerx,-centery)
  of.popMatrix()
  of.pushMatrix()
    of.translate(225, 675, 0)
    of.scale(5, 5, 1)
    of.setHexColor(0x333333)
    verdana:drawString("scale 5x!", 0, 0)
  of.popMatrix()
  local size = 2 + 2*math.sin(counter/300)
  of.pushMatrix()
    of.translate(520, 675, 0)
    of.scale(size, size, 1)
    of.setHexColor(0x00FF00)
    franklinBook:drawString("$k@!%", 0, 0)
  of.popMatrix()
end