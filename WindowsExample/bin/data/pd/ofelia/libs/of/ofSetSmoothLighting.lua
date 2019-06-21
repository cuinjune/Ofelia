local log = pdLog("ofSetSmoothLighting")
local canvas = pdCanvas(this)
local bSmoothLighting = true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    bSmoothLighting = args[1] ~= 0
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetSmoothLighting(bSmoothLighting)
  return nil
end

function ofelia.bSmoothLighting(b)
  bSmoothLighting = b ~= 0
end