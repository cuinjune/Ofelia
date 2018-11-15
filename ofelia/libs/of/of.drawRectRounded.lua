local objName = "of.drawRectRounded"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x, y, z, width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = 0, 0, 0, 0, 0, 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 5 then
    x, y, width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = args[1], args[2], args[3], args[4], args[5], args[5], args[5], args[5]
  elseif #args == 6 then
    x, y, z, width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = args[1], args[2], args[3], args[4], args[5], args[6], args[6], args[6], args[6]
  elseif #args == 8 then
    x, y, width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]
  elseif #args == 9 then
    x, y, z, width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 5, 6, 8 or 9 creation arguments")
  end
end

function ofelia.bang()
  of.drawRectRounded(x, y, z, width, height, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius)
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

function ofelia.width(f)
  width = f
end

function ofelia.height(f)
  height = f
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

function ofelia.radius(f)
  topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius = f, f, f, f
end

function ofelia.pos(p)
  x, y, z = p.x, p.y, p.z
end

function ofelia.rectangle(p)
  x, y, width, height = p.x, p.y, p.width, p.height
end