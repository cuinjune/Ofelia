local objName = "ofDrawImage"
local log = pdLog()
local canvas = pdCanvas(this)
local name, pos, width, height = "", ofVec3f(), nil, nil

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args == 3 then
    name = args[1]
    pos:set(args[2], args[3])
  elseif #args == 4 then
    name = args[1]
    pos:set(args[2], args[3], args[4])
  elseif #args == 5 then
    name, width, height = args[1], args[4], args[5]
    pos:set(args[2], args[3])
  elseif #args == 6 then
    name, width, height = args[1], args[5], args[6]
    pos:set(args[2], args[3], args[4])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 3, 4, 5 or 6 creation arguments")
  end
end

function ofelia.bang()
  local m = require(name)
  if type(width) == "nil" or type(height) == "nil" then
    m.image:draw(pos:vec3())
    return nil
  end
  m.image:draw(pos:vec3(), width, height)
  return nil
end

function ofelia.name(s)
  name = s
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

function ofelia.pos(fv)
  if #fv == 2 then
    pos:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    pos:set(fv[1], fv[2], fv[3])
  else
    log:error(objName .. " : 'pos' requires 2 or 3 arguments")
  end
end