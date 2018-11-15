local objName = "of.setFrameRate"
local log = pd.Log()
local canvas = pd.Canvas(this)
local targetRate = 60

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    targetRate = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setFrameRate(targetRate)
  return nil
end

function ofelia.targetRate(i)
  targetRate = i
end