local objName = "of.sleepMillis"
local log = pd.Log()
local canvas = pd.Canvas(this)
local millis = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    millis = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.sleepMillis(millis)
  return nil
end

function ofelia.millis(i)
  millis = i
end