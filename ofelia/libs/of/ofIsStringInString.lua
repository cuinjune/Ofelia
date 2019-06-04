local objName = "ofIsStringInString"
local log = pdLog()
local canvas = pdCanvas(this)
local needle = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    needle = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.symbol(s)
  return ofIsStringInString(s, needle)
end

function ofelia.needle(s)
  needle = s
end