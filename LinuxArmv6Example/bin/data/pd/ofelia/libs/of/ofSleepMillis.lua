local log = pdLog("ofSleepMillis")
local canvas = pdCanvas(this)
local millis = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    millis = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSleepMillis(millis)
  return nil
end

function ofelia.millis(i)
  millis = i
end