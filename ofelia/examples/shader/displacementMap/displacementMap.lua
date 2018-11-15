local window = pd.OFWindow()
local canvas = pd.Canvas(this)
local clock = pd.Clock(this, "setup")
local shaderDir = canvas:getDir() .. "/data/"
local shader = of.Shader()
local plane = of.PlanePrimitive()
local img = of.Image()

function ofelia.new()
  pd.OFWindow.addListener("setup", this)
  pd.OFWindow.addListener("update", this)
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
  pd.OFWindow.removeListener("update", this)
  pd.OFWindow.removeListener("draw", this)
  pd.OFWindow.removeListener("exit", this)
end

function ofelia.setup()
  of.setWindowTitle("displacement map")
  of.enableArbTex()
  of.background(55)
  if of.isGLProgrammableRenderer() then
    shader:load(shaderDir .. "shadersGL3/shader")
  else
    shader:load(shaderDir .. "shadersGL2/shader")
  end
  img:allocate(80, 60, of.IMAGE_GRAYSCALE)
  plane:set(800, 600, 80, 60)
  plane:mapTexCoordsFromTexture(img:getTexture())
end

function ofelia.update()
  local noiseScale = of.map(of.getMouseX(), 0, of.getWidth(), 0, 0.1)
  local noiseVel = of.getElapsedTimef()
  local pixels = img:getPixels()
  local w = img:getWidth()
  local h = img:getHeight()
  for y=0, h-1 do
    for x=0, w-1 do    
      local i = y * w + x
      local noiseVelue = of.noise(x * noiseScale, y * noiseScale, noiseVel)
      pixels:setColor(i, of.Color(255 * noiseVelue))
    end
  end
  img:update()
end

function ofelia.draw()
  img:getTexture():bind()
  shader:beginShader()
  of.pushMatrix()
  local tx = of.getWidth() / 2
  local ty = of.getHeight() / 2
  of.translate(tx, ty)
  local percentY = of.getMouseY() / of.getHeight()
  local rotation = of.map(percentY, 0, 1, -60, 60, true) + 60
  of.rotateDeg(rotation, 1, 0, 0)
  plane:drawWireframe()
  of.popMatrix()
  shader:endShader()
  img:getTexture():unbind()
  of.setColor(255)
  img:draw(0, 0)
end

function ofelia.exit()
  shader:unload()
  img:clear()
end