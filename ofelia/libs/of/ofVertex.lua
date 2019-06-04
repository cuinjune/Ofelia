local objName = "ofVertex"
local log = pdLog()
local canvas = pdCanvas(this)
local pos = ofVec3f()

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    pos:set(args[1], args[2])
  elseif #args == 3 then
    pos:set(args[1], args[2], args[3])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 or 3 creation arguments")
  end
end

function ofelia.bang()
  ofVertex(pos:vec3())
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

function ofelia.pos(fv)
  if #fv == 2 then
    pos:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    pos:set(fv[1], fv[2], fv[3])
  else
    log:error(objName .. " : 'pos' requires 2 or 3 arguments")
  end
end