if type(window) ~= "userdata" then
  window = ofWindow()
end

local canvas = ofCanvas(this)
local clock = ofClock(this, "setup")
local shaderDir = canvas:getDir() .. "/data/"
local shader = ofShader()

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
  ofSetWindowTitle("simple color quad")
  ofBackground(255, 255, 255, 255)
  local platform = ofGetTargetPlatform()
  if platform == OF_TARGET_LINUXARMV6L or platform == OF_TARGET_LINUXARMV7L or platform == OF_TARGET_ANDROID or platform == OF_TARGET_IOS or platform == OF_TARGET_EMSCRIPTEN then
    shader:load(shaderDir .. "shadersES2/shader")
  elseif ofIsGLProgrammableRenderer() then
    shader:load(shaderDir .. "shadersGL3/shader")
  else
    shader:load(shaderDir .. "shadersGL2/shader")
  end
end

function M.draw()
  ofSetColor(255)
  shader:beginShader()
  ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight())
  shader:endShader()
end

function M.exit()
  shader:unload()
end