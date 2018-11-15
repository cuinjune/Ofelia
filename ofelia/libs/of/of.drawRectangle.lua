local objName = "of.drawRectangle"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x, y, z, width, height = 0, 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 4 then
    x, y, width, height  = args[1], args[2], args[3], args[4]
  elseif #args == 5 then
    x, y, z, width, height = args[1], args[2], args[3], args[4], args[5]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 4 or 5 creation arguments")
  end
end

function ofelia.bang()
  of.drawRectangle(x, y, z, width, height)
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

function ofelia.pos(p)
  x, y, z = p.x, p.y, p.z
end

function ofelia.rectangle(p)
  x, y, width, height = p.x, p.y, p.width, p.height
end