local objName = "of.setTimeModeFixedRate"
local log = pd.Log()
local canvas = pd.Canvas(this)
local stepNanos = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    stepNanos = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setTimeModeFixedRate(stepNanos)
  return nil
end

function ofelia.stepNanos(i)
  stepNanos = i
end