local log = pdLog("ofLerpDegrees")
local canvas = pdCanvas(this)
local currentAngle, targetAngle = 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    currentAngle, targetAngle = args[1], args[2]
  elseif #args ~= 0 then
    log:error("requires 2 creation arguments")
  end
end

function ofelia.float(f)
  return ofLerpDegrees(currentAngle, targetAngle, f)
end

function ofelia.currentAngle(f)
  currentAngle = f
end

function ofelia.targetAngle(f)
  targetAngle = f
end