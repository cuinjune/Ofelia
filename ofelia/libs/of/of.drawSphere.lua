local objName = "of.drawSphere"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x, y, z, radius = 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    radius = args[1]
  elseif #args == 3 then
    x, y, radius = args[1], args[2], args[3]
  elseif #args == 4 then
    x, y, z, radius = args[1], args[2], args[3], args[4]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 3 or 4 creation arguments")
  end
end

function ofelia.bang()
  of.drawSphere(x, y, z, radius)
  return nil
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

function ofelia.radius(f)
  radius = f
end

function ofelia.pos(p)
  x, y, z = p.x, p.y, p.z
end