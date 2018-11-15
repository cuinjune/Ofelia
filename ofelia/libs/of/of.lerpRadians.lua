local objName = "of.lerpRadians"
local log = pd.Log()
local canvas = pd.Canvas(this)
local currentAngle, targetAngle, pct = 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    currentAngle, targetAngle, pct = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 3 creation arguments")
  end
end

function ofelia.bang()
  return of.lerpRadians(currentAngle, targetAngle, pct)
end

function ofelia.currentAngle(f)
  currentAngle = f
end

function ofelia.targetAngle(f)
  targetAngle = f
end

function ofelia.pct(f)
  pct = f
end