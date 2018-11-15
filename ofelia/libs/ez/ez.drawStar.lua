local objName = "ez.drawStar"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x, y, z, radius1, radius2, npoints = 0, 0, 0, 0, 0, 0
local twopi = math.pi * 2

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 5 then
    x, y, radius1, radius2, npoints  = args[1], args[2], args[3], args[4], args[5]
  elseif #args == 6 then
    x, y, z, radius1, radius2, npoints = args[1], args[2], args[3], args[4], args[5], args[6]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 5 or 6 creation arguments")
  end
end

function ofelia.bang()
  local angle = twopi / npoints
  local halfAngle = angle / 2
  of.beginShape()
  for a = 0, twopi, angle do
    local sx = x + math.cos(a) * radius2
    local sy = y + math.sin(a) * radius2
    of.vertex(sx, sy, z)
    sx = x + math.cos(a+halfAngle) * radius1
    sy = y + math.sin(a+halfAngle) * radius1
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

function ofelia.radius1(f)
  radius1 = f
end

function ofelia.radius2(f)
  radius2 = f
end

function ofelia.npoints(i)
  npoints = i
end

function ofelia.pos(p)
  x, y, z = p.x, p.y, p.z
end