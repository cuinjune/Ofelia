local objName = "ofRotateYDeg"
local log = pdLog()
local canvas = pdCanvas(this)
local degrees = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    degrees = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  ofRotateYDeg(degrees)
  return nil
end

function ofelia.degrees(f)
  degrees = f
end