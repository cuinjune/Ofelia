local objName = "of.distSquared"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x1, y1, z1, x2, y2, z2 = 0, 0, 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 4 then
    x1, y1, x2, y2 = args[1], args[2], args[3], args[4]
  elseif #args == 6 then
    x1, y1, z1, x2, y2, z2 = args[1], args[2], args[3], args[4], args[5], args[6]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 4 or 6 creation arguments")
  end
end

function ofelia.bang()
  return of.distSquared(x1, y1, z1, x2, y2, z2)
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