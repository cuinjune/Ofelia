if type(window) ~= "userdata" then
  window = ofWindow()
end

local canvas = pdCanvas(this)
local clock = pdClock(this, "setup")
local shaderDir = canvas:getDir() .. "/data/"
local shader = ofShader()

function ofelia.new()
  ofWindow.addListener("setup", this)
  ofWindow.addListener("draw", this)
  ofWindow.addListener("exit", this)
  window:setPosition(30, 100)
  window:setSize(1024, 768)
  window:create()
  if ofWindow.exists then
    clock:delay(0)
  end
end

function ofelia.free()
  window:destroy()
  ofWindow.removeListener("setup", this)
  ofWindow.removeListener("draw", this)
  ofWindow.removeListener("exit", this)
end

function ofelia.setup()
  ofSetWindowTitle("simple color quad")
  ofBackground(255, 255, 255, 255)
  if ofIsGLProgrammableRenderer() then
    shader:load(shaderDir .. "shadersGL3/shader")
  else
    shader:load(shaderDir .. "shadersGL2/shader")
  end
end

function ofelia.draw()
  ofSetColor(255)
  shader:beginShader()
  ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight())
  shader:endShader()
end

function ofelia.exit()
  shader:unload()
end