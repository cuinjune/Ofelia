local objName = "of.bezierVertex"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x1, y1, z1, x2, y2, z2, x3, y3, z3 = 0, 0, 0, 0, 0, 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 6 then
    x1, y1, x2, y2, x3, y3 = args[1], args[2], args[3], args[4], args[5], args[6]
  elseif #args == 9 then
    x1, y1, z1, x2, y2, z2, x3, y3, z3 = args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 6 or 9 creation arguments")
  end
end

function ofelia.bang()
  of.bezierVertex(x1, y1, z1, x2, y2, z2, x3, y3, z3)
  return nil
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

function ofelia.p1(p)
  x1, y1, z1 = p.x, p.y, p.z
end

function ofelia.p2(p)
  x2, y2, z2 = p.x, p.y, p.z
end

function ofelia.p3(p)
  x3, y3, z3 = p.x, p.y, p.z
end