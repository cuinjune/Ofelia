local objName = "of.TrueTypeFont.drawString"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local str, x, y = "", 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    str, x, y = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 3 creation arguments")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return nil
  end
  font:drawString(str, x, y)
  return nil
end

function ofelia.str(s)
  str = s
end

function ofelia.x(f)
  x = f
end

function ofelia.y(f)
  y = f
end

function ofelia.font(p)
  font = p
end