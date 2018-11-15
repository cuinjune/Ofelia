local objName = "of.drawBitmapString"
local log = pd.Log()
local canvas = pd.Canvas(this)
local text, x, y, z = "", 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    text, x, y = args[1], args[2], args[3]
  elseif #args == 4 then
    text, x, y, z = args[1], args[2], args[3], args[4]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 3 or 4 creation arguments")
  end
end

function ofelia.bang()
  of.drawBitmapString(text, x, y, z)
  return nil
end

function ofelia.text(s)
  text = s
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