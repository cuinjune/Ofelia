local objName = "ofDrawRectRounded"
local log = pdLog()
local canvas = pdCanvas(this)
local pos, width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = ofVec3f(), 0, 0, 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 5 then
    pos:set(args[1], args[2])
    width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = args[3], args[4], args[5], args[5], args[5], args[5]
  elseif #args == 6 then
    pos:set(args[1], args[2], args[3])
    width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = args[4], args[5], args[6], args[6], args[6], args[6]
  elseif #args == 8 then
    pos:set(args[1], args[2])
    width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = args[3], args[4], args[5], args[6], args[7], args[8]
  elseif #args == 9 then
    pos:set(args[1], args[2], args[3])
    width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = args[4], args[5], args[6], args[7], args[8], args[9]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 5, 6, 8 or 9 creation arguments")
  end
end

function ofelia.bang()
  ofDrawRectRounded(pos:vec3(), width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius)
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

function ofelia.width(f)
  width = f
end

function ofelia.height(f)
  height = f
end

function ofelia.radius(f)
  topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = f, f, f, f
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

function ofelia.rectangle(fv)
  pos:set(fv[1], fv[2])
  width, height = fv[3], fv[4]
end

function ofelia.topLeftRadius(f)
  topLeftRadius = f
end

function ofelia.topRightRadius(f)
  topRightRadius = f
end

function ofelia.bottomRightRadius(f)
  bottomRightRadius = f
end

function ofelia.bottomLeftRadius(f)
  bottomLeftRadius = f
end