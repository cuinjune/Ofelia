local objName = "of.drawBox"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x, y, z, width, height, depth = 0, 0, 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    width, height, depth = args[1], args[1], args[1]
  elseif #args == 3 then
    width, height, depth = args[1], args[2], args[3]
  elseif #args == 4 then
    x, y, z, width, height, depth = args[1], args[2], args[3], args[4], args[4], args[4]
  elseif #args == 6 then
    x, y, z, width, height, depth = args[1], args[2], args[3], args[4], args[5], args[6]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 3, 4 or 6 creation arguments")
  end
end

function ofelia.bang()
  of.drawBox(x, y, z, width, height, depth)
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

function ofelia.depth(f)
  depth = f
end

function ofelia.pos(p)
  x, y, z = p.x, p.y, p.z
end