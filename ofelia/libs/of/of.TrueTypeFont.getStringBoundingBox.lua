local objName = "of.TrueTypeFont.getStringBoundingBox"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local str, x, y, vFlip = "", 0, 0, true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    str, x, y = args[1], args[2], args[3]
  elseif #args == 4 then
    str, x, y, vFlip = args[1], args[2], args[3], args[4] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 3 or 4 creation arguments")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return of.Rectangle()
  end
  return font:getStringBoundingBox(str, x, y, vFlip)
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

function ofelia.vFlip(b)
  vFlip = b ~= 0
end

function ofelia.font(p)
  font = p
end