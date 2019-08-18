if type(window) ~= "userdata" then
  window = ofWindow()
end

local canvas = ofCanvas(this)
local clock = ofClock(this, "setup")
local shaderDir = canvas:getDir() .. "/data/shaders/"
local shader = ofShader()
local doShader = false
local points = {}

function M.new()
  ofWindow.addListener("setup", this)
  ofWindow.addListener("draw", this)
  ofWindow.addListener("keyPressed", this)
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
  ofWindow.removeListener("keyPressed", this)
  ofWindow.removeListener("exit", this)
end

function M.setup()
  ofBackground(50)
  ofSetVerticalSync(false)
  ofEnableAlphaBlending()
  shader:setGeometryInputType(GL_LINES)
  shader:setGeometryOutputType(GL_TRIANGLE_STRIP)
  shader:setGeometryOutputCount(4)
  shader:load(shaderDir .. "vert.glsl", shaderDir .. "frag.glsl", shaderDir .. "geom.glsl")
  local r = ofGetHeight()/2
  for i=1, 100 do
    points[i] = ofPoint(ofRandomf() * r, ofRandomf() * r, ofRandomf() * r)
  end
  doShader = true
  ofEnableDepthTest()
end

function M.draw()
  ofPushMatrix()
  if doShader then
    shader:beginShader()
    shader:setUniform1f("thickness", 20)
    shader:setUniform3f("lightDir", math.sin(ofGetElapsedTimef()/10), math.cos(ofGetElapsedTimef()/10), 0)
  end
  ofSetColor(255)
  ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0)
  ofRotateXDeg(ofGetMouseY())
  ofRotateYDeg(ofGetMouseX())
  for i=2, #points do
    ofDrawLine(points[i-1]:vec3(), points[i]:vec3())
  end
  if doShader then
    shader:endShader()
  end
  ofPopMatrix()
  local toggle = "OFF"
  if doShader then
    toggle = "ON"
  end
  ofDrawBitmapString("fps: " .. string.format("%.0f", ofGetFrameRate()) .. string.char(10) .. "Press 's' to toggle shader: " .. toggle, 20, 20)
end

function M.keyPressed(e)
  if e.key == string.byte("s") then
    doShader = not doShader
  end
end

function M.exit()
  shader:unload()
end