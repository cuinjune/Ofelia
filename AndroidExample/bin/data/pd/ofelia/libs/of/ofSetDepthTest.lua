local log = pdLog("ofSetDepthTest")
local canvas = pdCanvas(this)
local depthTest = false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    depthTest = args[1] ~= 0
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetDepthTest(depthTest)
  return nil
end

function ofelia.depthTest(b)
  depthTest = b ~= 0
end