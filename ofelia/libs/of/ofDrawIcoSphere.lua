local objName = "ofDrawIcoSphere"
local log = pdLog()
local canvas = pdCanvas(this)
local pos, radius = ofVec3f(), 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    radius = args[1]
  elseif #args == 3 then
    pos:set(args[1], args[2])
    radius = args[3]
  elseif #args == 4 then
    pos:set(args[1], args[2], args[3])
    adius = args[4]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 3 or 4 creation arguments")
  end
end

function ofelia.bang()
  ofDrawIcoSphere(pos:vec3(), radius)
  return nil
end

function ofelia.x(f)
  pos.x = f
end

function ofelia.y(f)
  pos.y = f
end

function ofelia.z(f)
  pos.z = f
end

function ofelia.radius(f)
  radius = f
end

function ofelia.pos(fv)
  if #fv == 2 then
    pos:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    pos:set(fv[1], fv[2], fv[3])
  else
    log:error(objName .. " : 'pos' requires 2 or 3 arguments")
  end
end