local objName = "ez.drawImage"
local log = pd.Log()
local canvas = pd.Canvas(this)
local name, x, y, z, width, height = "$1", 0, 0, 0, nil, nil

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args == 3 then
    name, x, y = args[1], args[2], args[3]
  elseif #args == 4 then
    name, x, y, z = args[1], args[2], args[3], args[4]
  elseif #args == 5 then
    name, x, y, width, height = args[1], args[2], args[3], args[4], args[5]
  elseif #args == 6 then
    name, x, y, z, width, height = args[1], args[2], args[3], args[4], args[5], args[6]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 3, 4, 5 or 6 creation arguments")
  end
end

function ofelia.bang()
  local m = require("ez." .. name)
  if type(m.image) ~= "userdata" then
    return nil
  end
  if type(width) == "nil" or type(height) == "nil" then
    m.image:draw(x, y, z)
    return nil
  end
  m.image:draw(x, y, z, width, height)
  return nil
end

function ofelia.name(s)
  name = s
end

function ofelia.x(f)
  x = f
end

function ofelia.y(f)
  y = f
end

function ofelia.z(f)
  z = f
end

function ofelia.width(f)
  width = f
end

function ofelia.height(f)
  height = f
end

function ofelia.pos(p)
  x, y, z = p.x, p.y, p.z
end