if type(window) ~= "userdata" then
  window = pd.OFWindow()
end

local clock = pd.Clock(this, "setup")
local counter = 0
local bSmooth = false

function ofelia.new()
  pd.OFWindow.addListener("setup", this)
  pd.OFWindow.addListener("update", this)
  pd.OFWindow.addListener("draw", this)
  pd.OFWindow.addListener("keyPressed", this)
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
end

function ofelia.setup()
  of.setWindowTitle("graphics example")
  of.setCircleResolution(50)
  of.background(255, 255, 255, 255)
  of.disableSmoothing()
end

function ofelia.update()
  counter = counter + 0.033
end

function ofelia.draw()
  of.setColor(255, 130, 0)
  local radius = 50 + 10 * math.sin(counter)
  of.fill()
  of.drawCircle(100, 400, radius)
  of.noFill()
  of.setHexColor(0xCCCCCC)
  of.drawCircle(100, 400, 80)
  of.setHexColor(0x000000)
  of.drawBitmapString("circle", 75, 500)
  of.fill()
  for i=0,200 do
    of.setColor(of.random(0, 255), of.random(0, 255), of.random(0, 255))
    of.drawRectangle(of.random(250, 350), of.random(350, 450), of.random(10, 20), of.random(10, 20))
  end
  of.setHexColor(0x000000)
  of.drawBitmapString("rectangles", 275, 500)
  of.setHexColor(0x00FF33)
  of.drawRectangle(400, 350, 100, 100)
  of.enableAlphaBlending()
  of.setColor(255, 0, 0, 127)
  of.drawRectangle(450, 430, 100, 33)
  of.setColor(255, 0, 0, math.fmod(counter*10, 255))
  of.drawRectangle(450, 370, 100, 33)
  of.disableAlphaBlending()
  of.setHexColor(0x000000)
  of.drawBitmapString("transparency", 410, 500)
  if bSmooth then
    of.enableSmoothing()
  end
  of.setHexColor(0xFF0000)
    for i=0,20 do
      of.drawLine(600, 300 + (i*5), 800, 250 + (i*10))
    end
  if bSmooth then
    of.disableSmoothing()
  end
  of.setHexColor(0x000000)
  of.drawBitmapString("lines"..string.char(10).."press 's' to toggle smoothness", 600, 500)
end

function ofelia.keyPressed(e)
  if e.key == string.byte("s") then
    bSmooth = not bSmooth
  end
end