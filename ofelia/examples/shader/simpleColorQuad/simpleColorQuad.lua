local window = pd.OFWindow()
local canvas = pd.Canvas(this)
local clock = pd.Clock(this, "setup")
local shaderDir = canvas:getDir() .. "/data/"
local shader = of.Shader()

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
  of.setWindowTitle("simple color quad")
  of.background(255, 255, 255, 255)
  if of.isGLProgrammableRenderer() then
    shader:load(shaderDir .. "shadersGL3/shader")
  else
    shader:load(shaderDir .. "shadersGL2/shader")
  end
end

function ofelia.draw()
  of.setColor(255)
  shader:beginShader()
  of.drawRectangle(0, 0, of.getWidth(), of.getHeight())
  shader:endShader()
end

function ofelia.exit()
  shader:unload()
end