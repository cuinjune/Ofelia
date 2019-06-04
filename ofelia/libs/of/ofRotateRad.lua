local objName = "ofRotateRad"
local log = pdLog()
local canvas = pdCanvas(this)
local degrees, vecX, vecY, vecZ = 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    degrees, vecX, vecY, vecZ = args[1], 0, 0, 1
  elseif #args == 4 then
    degrees, vecX, vecY, vecZ = args[1], args[2], args[3], args[4]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 4 creation arguments")
  end
end

function ofelia.bang()
  ofRotateRad(degrees, vecX, vecY, vecZ)
  return nil
end

function ofelia.degrees(f)
  degrees = f
end

function ofelia.vecX(f)
  vecX = f
end

function ofelia.vecY(f)
  vecY = f
end

function ofelia.vecZ(f)
  vecZ = f
end