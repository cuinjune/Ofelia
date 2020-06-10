if type(window) ~= "userdata" then
  window = ofWindow()
end

local canvas = ofCanvas(this)
local clock = ofClock(this, "setup")
local shaderDir = canvas:getDir() .. "/data/"
local shader = ofShader()
local plane = ofPlanePrimitive()
local img = ofImage()

function M.new()
  ofWindow.addListener("setup", this)
  ofWindow.addListener("update", this)
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
  ofWindow.removeListener("update", this)
  ofWindow.removeListener("draw", this)
  ofWindow.removeListener("exit", this)
end

function M.setup()
  ofSetWindowTitle("displacement map")
  ofEnableArbTex()
  ofBackground(55)
  local platform = ofGetTargetPlatform()
  if platform == OF_TARGET_LINUXARMV6L or platform == OF_TARGET_LINUXARMV7L or platform == OF_TARGET_ANDROID or platform == OF_TARGET_IOS or platform == OF_TARGET_EMSCRIPTEN then
    shader:load(shaderDir .. "shadersES2/shader")
  elseif ofIsGLProgrammableRenderer() then
    shader:load(shaderDir .. "shadersGL3/shader")
  else
    shader:load(shaderDir .. "shadersGL2/shader")
  end
  img:allocate(80, 60, OF_IMAGE_GRAYSCALE)
  plane:set(800, 600, 80, 60)
  plane:mapTexCoordsFromTexture(img:getTexture())
end

function M.update()
  local noiseScale = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 0.1)
  local noiseVel = ofGetElapsedTimef()
  local pixels = img:getPixels()
  local w = img:getWidth()
  local h = img:getHeight()
  for y=0, h-1 do
    for x=0, w-1 do    
      local i = y * w + x
      local noiseValue = ofNoise(x * noiseScale, y * noiseScale, noiseVel)
      pixels:setColor(i, ofColor(255 * noiseValue))
    end
  end
  img:update()
end

function M.draw()
  img:getTexture():bind()
  shader:beginShader()
  ofPushMatrix()
  local tx = ofGetWidth() / 2
  local ty = ofGetHeight() / 2
  ofTranslate(tx, ty)
  local percentY = ofGetMouseY() / ofGetHeight()
  local rotation = ofMap(percentY, 0, 1, -60, 60, true) + 60
  ofRotateDeg(rotation, 1, 0, 0)
  plane:drawWireframe()
  ofPopMatrix()
  shader:endShader()
  img:getTexture():unbind()
  ofSetColor(255)
  img:draw(0, 0)
end

function M.exit()
  shader:unload()
  plane:setUseVbo(false)
  img:clear()
end