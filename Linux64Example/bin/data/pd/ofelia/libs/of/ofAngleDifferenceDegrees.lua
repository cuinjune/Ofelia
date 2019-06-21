local log = pdLog("ofAngleDifferenceDegrees")
local canvas = pdCanvas(this)
local targetAngle = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    targetAngle = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.float(f)
  return ofAngleDifferenceDegrees(f, targetAngle)
end

function ofelia.targetAngle(f)
  targetAngle = f
end