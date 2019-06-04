local objName = "ofDist"
local log = pdLog()
local canvas = pdCanvas(this)
local p2 = ofVec3f()

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    p2:set(args[1], args[2])
  elseif #args == 3 then
    p2:set(args[1], args[2], args[3])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 or 3 creation arguments")
  end
end

function ofelia.list(fv)
  if #fv == 2 then
    return ofDist(fv[1], fv[2], 0, p2.x, p2.y, p2.z)
  elseif #fv == 3 then
    return ofDist(fv[1], fv[2], fv[3], p2.x, p2.y, p2.z)
  else
    log:error(objName .. " : requires 2 or 3 arguments")
  end
end

function ofelia.p2(fv)
  if #fv == 2 then
    p2:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    p2:set(fv[1], fv[2], fv[3])
  else
    log:error(objName .. " : 'p2' requires 2 or 3 arguments")
  end
end