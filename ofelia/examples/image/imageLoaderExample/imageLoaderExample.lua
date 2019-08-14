if type(window) ~= "userdata" then
  window = ofWindow()
end

local canvas = ofCanvas(this)
local clock = ofClock(this, "setup")
local imageDir = canvas:getDir() .. "/data/"
local bikers = ofImage()
local gears = ofImage()
local tdf = ofImage()
local tdfSmall = ofImage()
local transparency = ofImage()
local bikeIcon = ofImage()

function M.new()
  ofWindow.addListener("setup", this)
  ofWindow.addListener("draw", this)
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
  ofWindow.removeListener("draw", this)
  ofWindow.removeListener("exit", this)
end

function M.setup()
  ofSetWindowTitle("image loader example")
  ofBackground(255, 255, 255, 255)
  bikers:load(imageDir .. "bikers.jpg")
  gears:load(imageDir .. "gears.gif")
  tdf:load(imageDir .. "tdf_1972_poster.jpg")
  tdfSmall:load(imageDir .."tdf_1972_poster.jpg")
  tdfSmall:resize(tdfSmall:getWidth() / 4, tdfSmall:getHeight() / 4)
  tdfSmall:setImageType(OF_IMAGE_GRAYSCALE)
  transparency:load(imageDir .. "transparency.png")
  bikeIcon:load(imageDir .. "bike_icon.png")
  bikeIcon:setImageType(OF_IMAGE_GRAYSCALE)
end

function M.draw()
  ofSetColor(255)
  bikers:draw(0, 0)
  gears:draw(600, 0)
  tdf:draw(600, 300)
  ofSetColor(220, 50, 50)
  tdfSmall:draw(200, 300)
  ofSetColor(255)
  ofEnableAlphaBlending()
  local wave = math.sin(ofGetElapsedTimef())
  transparency:draw(500 + (wave * 100), 20)
  ofDisableAlphaBlending()
  local w = bikeIcon:getWidth()
  local h = bikeIcon:getHeight()
  local diameter = 10
  ofSetColor(255, 0, 0)
  for y=1,h-1 do
    for x=1,w-1 do
      local cur = bikeIcon:getColor(x, y)
      local size = 1 - (cur:getBrightness() / 255)
      ofDrawCircle(x * diameter, 500 + y * diameter, 1 + size * diameter / 2)
      cur = nil
    end
  end
  local pixels = bikeIcon:getPixels()
  ofSetColor(0, 0, 255)
  for y=1,h-1 do
    for x=1,w-1 do
      local index = y * w + x
      cur = pixels:getColor(index).r
      size = 1 - (cur / 255)
      ofDrawCircle(200 + x * diameter, 500 + y * diameter, 1 + size * diameter / 2)
    end
  end
  ofSetColor(255)
  bikeIcon:draw(190, 490, 20, 20)
end

function M.exit()
  bikers:clear()
  gears:clear()
  tdf:clear()
  tdfSmall:clear()
  transparency:clear()
  bikeIcon:clear()
end