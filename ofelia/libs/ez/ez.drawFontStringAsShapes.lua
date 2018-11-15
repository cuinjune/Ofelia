local objName = "ez.drawFontStringAsShapes"
local log = pd.Log()
local canvas = pd.Canvas(this)
local name, str, x, y = "$1", "", 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args == 4 then
    name, str, x, y = args[1], args[2], args[3], args[4]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 4 creation arguments")
  end
end

function ofelia.bang()
  local m = require("ez." .. name)
  if type(m.font) ~= "userdata" then
    return nil
  end
  m.font:drawStringAsShapes(str, x, y)
  return nil
end

function ofelia.name(s)
  name = s
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