local objName = "ofDrawBezier"
local log = pdLog()
local canvas = pdCanvas(this)
local p0, p1, p2, p3 = ofVec3f(), ofVec3f(), ofVec3f(), ofVec3f()

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 8 then
    p0:set(args[1], args[2])
    p1:set(args[3], args[4])
    p2:set(args[5], args[6])
    p3:set(args[7], args[8])
  elseif #args == 12 then
    p0:set(args[1], args[2], args[3])
    p1:set(args[4], args[5], args[6])
    p2:set(args[7], args[8], args[9])
    p3:set(args[10], args[11], args[12])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 8 or 12 creation arguments")
  end
end

function ofelia.bang()
  ofDrawBezier(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z)
  return nil
end

function ofelia.p0(fv)
  if #fv == 2 then
    p0:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    p0:set(fv[1], fv[2], fv[3])
  else
    log:error(objName .. " : 'p0' requires 2 or 3 arguments")
  end
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