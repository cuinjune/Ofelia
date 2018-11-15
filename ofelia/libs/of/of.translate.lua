local objName = "of.translate"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x, y, z = 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    x, y = args[1], args[2]
  elseif #args == 3 then
    x, y, z = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 or 3 creation arguments")
  end
end

function ofelia.bang()
  of.translate(x, y, z)
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

function ofelia.pos(p)
  x, y, z = p.x, p.y, p.z
end