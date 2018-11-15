local objName = "ez.drawPolygon"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x, y, z, radius, npoints = 0, 0, 0, 0, 0
local twopi = math.pi * 2

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 4 then
    x, y, radius, npoints  = args[1], args[2], args[3], args[4]
  elseif #args == 5 then
    x, y, z, radius, npoints = args[1], args[2], args[3], args[4], args[5]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 4 or 5 creation arguments")
  end
end

function ofelia.bang()
  local angle = twopi / npoints
  of.beginShape()
  for a = 0, twopi, angle do
    local sx = x + math.cos(a) * radius
    local sy = y + math.sin(a) * radius
    of.vertex(sx, sy, z)
  end
  of.endShape(true)
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

function ofelia.npoints(i)
  npoints = i
end

function ofelia.pos(p)
  x, y, z = p.x, p.y, p.z
end