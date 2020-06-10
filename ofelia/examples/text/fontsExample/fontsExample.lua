if type(window) ~= "userdata" then
  window = ofWindow()
end

local canvas = ofCanvas(this)
local clock = ofClock(this, "setup")
local fontDir = canvas:getDir() .. "/data/"
local franklinBook = ofTrueTypeFont()
local verdana = ofTrueTypeFont()
local franklinBookLarge = ofTrueTypeFont()
local counter = 0

function M.new()
  ofWindow.addListener("setup", this)
  ofWindow.addListener("update", this)
  ofWindow.addListener("draw", this)
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
end

function M.setup()
  ofSetWindowTitle("fonts example")
  franklinBook:load(fontDir .. "frabk.ttf", 32)
  verdana:load(fontDir .. "verdana.ttf", 8, false, true)
  verdana:setLineHeight(20)
end

function M.update()
  ofBackground(255, 255, 255)	
  counter = counter + 1
end

function M.draw()
  ofSetHexColor(0x00FF00)
  franklinBook:drawString("hello, this is franklin book calling"..string.char(10).."anyone home?", 100, 100)
  ofSetHexColor(0x000000)
  verdana:drawString("hello, I am aliased verdana -- full character set, see: � ! ", 100, 210)
  ofSetHexColor(0x00FF00)
  franklinBook:drawString("I can't make an (�) like you", 100, 310)
  ofSetHexColor(0x000000)
  verdana:drawString("yeah, but I'm not exactly pretty"..string.char(10).."the problem is with freeType library..."..string.char(10).."apple has a patent on TTF font hints"..string.char(10).."so our aliased type via freeType isn't super looking", 100, 380)
  ofSetHexColor(0x00FF00)
  franklinBook:drawString("you look ok ! don't worry", 100, 520)
  tempString = tostring(counter)
  rect = franklinBook:getStringBoundingBox(tempString, 0, 0)
  local centerx = rect.x + rect.width / 2
  local centery = rect.y + rect.height / 2
  ofPushMatrix()
    ofTranslate(100, 650, 0)
    ofRotateDeg(counter, 0, 0, 1)
    ofSetHexColor(0xcccccc)
    ofDrawRectangle(rect.x - centerx, rect.y - centery, rect.width, rect.height)
    ofSetHexColor(0xff3399)
    franklinBook:drawString(tempString, -centerx,-centery)
  ofPopMatrix()
  ofPushMatrix()
    ofTranslate(225, 675, 0)
    ofScale(5, 5, 1)
    ofSetHexColor(0x333333)
    verdana:drawString("scale 5x!", 0, 0)
  ofPopMatrix()
  local size = 2 + 2*math.sin(counter/300)
  ofPushMatrix()
    ofTranslate(520, 675, 0)
    ofScale(size, size, 1)
    ofSetHexColor(0x00FF00)
    franklinBook:drawString("$k@!%", 0, 0)
  ofPopMatrix()
end