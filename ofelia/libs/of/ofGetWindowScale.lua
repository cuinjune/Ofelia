local objName = "ofGetWindowScale"
local log = pdLog()
local canvas = pdCanvas(this)
local name = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return ofWindow.scale
end

function ofelia.name(s)
  name = s
end