local objName = "of.drawCurve"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3 = 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 8 then
    x0, y0, x1, y1, x2, y2, x3, y3 = args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]
  elseif #args == 12 then
    x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3 = args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 8 or 12 creation arguments")
  end
end

function ofelia.bang()
  of.drawCurve(x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3)
  return nil
end

function ofelia.x0(f)
  x0 = f
end

function ofelia.y0(f)
  y0 = f
end

function ofelia.z0(f)
  z0 = f
end

function ofelia.x1(f)
  x1 = f
end

function ofelia.y1(f)
  y1 = f
end

function ofelia.z1(f)
  z1 = f
end

function ofelia.x2(f)
  x2 = f
end

function ofelia.y2(f)
  y2 = f
end

function ofelia.z2(f)
  z2 = f
end

function ofelia.x3(f)
  x3 = f
end

function ofelia.y3(f)
  y3 = f
end

function ofelia.z3(f)
  z3 = f
end

function ofelia.p0(p)
  x0, y0, z0 = p.x, p.y, p.z
end

function ofelia.p1(p)
  x1, y1, z1 = p.x, p.y, p.z
end

function ofelia.p2(p)
  x2, y2, z2 = p.x, p.y, p.z
end

function ofelia.p3(p)
  x3, y3, z3 = p.x, p.y, p.z
end