local log = pdLog("ofStringTimesInString")
local canvas = pdCanvas(this)
local needle = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    needle = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.symbol(s)
  return ofStringTimesInString(s, needle)
end

function ofelia.needle(s)
  needle = s
end