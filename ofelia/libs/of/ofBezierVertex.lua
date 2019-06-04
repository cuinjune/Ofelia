local objName = "ofBezierVertex"
local log = pdLog()
local canvas = pdCanvas(this)
local p1, p2, p3 = ofVec3f(), ofVec3f(), ofVec3f()

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 6 then
    p1:set(args[1], args[2])
    p2:set(args[3], args[4])
    p3:set(args[5], args[6])
  elseif #args == 9 then
    p1:set(args[1], args[2], args[3])
    p2:set(args[4], args[5], args[6])
    p3:set(args[7], args[8], args[9])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 6 or 9 creation arguments")
  end
end

function ofelia.bang()
  ofBezierVertex(p1:vec3(), p2:vec3(), p3:vec3())
  return nil
end

function ofelia.p1(fv)
  if #fv == 2 then
    p1:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    p1:set(fv[1], fv[2], fv[3])
  else
    log:error(objName .. " : 'p1' requires 2 or 3 arguments")
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

function ofelia.p3(fv)
  if #fv == 2 then
    p3:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    p3:set(fv[1], fv[2], fv[3])
  else
    log:error(objName .. " : 'p3' requires 2 or 3 arguments")
  end
end