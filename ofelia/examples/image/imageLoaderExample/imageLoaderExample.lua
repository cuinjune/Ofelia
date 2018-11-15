local window = pd.OFWindow()
local canvas = pd.Canvas(this)
local clock = pd.Clock(this, "setup")
local imageDir = canvas:getDir() .. "/data/"
local bikers = of.Image()
local gears = of.Image()
local tdf = of.Image()
local tdfSmall = of.Image()
local transparency = of.Image()
local bikeIcon = of.Image()

function ofelia.new()
  pd.OFWindow.addListener("setup", this)
  pd.OFWindow.addListener("draw", this)
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
  pd.OFWindow.removeListener("draw", this)
  pd.OFWindow.removeListener("exit", this)
end

function ofelia.setup()
  of.setWindowTitle("image loader example")
  of.background(255, 255, 255, 255)
  bikers:load(imageDir .. "bikers.jpg")
  gears:load(imageDir .. "gears.gif")
  tdf:load(imageDir .. "tdf_1972_poster.jpg")
  tdfSmall:load(imageDir .."tdf_1972_poster.jpg")
  tdfSmall:resize(tdfSmall:getWidth() / 4, tdfSmall:getHeight() / 4)
  tdfSmall:setImageType(of.IMAGE_GRAYSCALE)
  transparency:load(imageDir .. "transparency.png")
  bikeIcon:load(imageDir .. "bike_icon.png")
  bikeIcon:setImageType(of.IMAGE_GRAYSCALE)
end

function ofelia.draw()
  of.setColor(255)
  bikers:draw(0, 0)
  gears:draw(600, 0)
  tdf:draw(600, 300)
  of.setColor(220, 50, 50)
  tdfSmall:draw(200, 300)
  of.setColor(255)
  of.enableAlphaBlending()
  local wave = math.sin(of.getElapsedTimef())
  transparency:draw(500 + (wave * 100), 20)
  of.disableAlphaBlending()
  local w = bikeIcon:getWidth()
  local h = bikeIcon:getHeight()
  local diameter = 10
  of.setColor(255, 0, 0)
  for y=1,h-1 do
    for x=1,w-1 do
      local cur = bikeIcon:getColor(x, y)
      local size = 1 - (cur:getBrightness() / 255)
      of.drawCircle(x * diameter, 500 + y * diameter, 1 + size * diameter / 2)
      cur = nil
    end
  end
  local pixels = bikeIcon:getPixels()
  of.setColor(0, 0, 255)
  for y=1,h-1 do
    for x=1,w-1 do
      local index = y * w + x
      cur = pixels:getColor(index).r
      size = 1 - (cur / 255)
      of.drawCircle(200 + x * diameter, 500 + y * diameter, 1 + size * diameter / 2)
    end
  end
  of.setColor(255)
  bikeIcon:draw(190, 490, 20, 20)
end

function ofelia.exit()
  bikers:clear()
  gears:clear()
  tdf:clear()
  tdfSmall:clear()
  transparency:clear()
  bikeIcon:clear()
end