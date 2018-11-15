local objName = "of.TrueTypeFont.getFirstGlyphPosForTexture"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local str, vFlip = "", true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    str = args[1]
  elseif #args == 2 then
    str, vFlip = args[1], args[2] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return of.Vec2f()
  end
  return font:getFirstGlyphPosForTexture(str, vFlip)
end

function ofelia.str(s)
  str = s
end

function ofelia.vFlip(b)
  vFlip = b ~= 0
end

function ofelia.font(p)
  font = p
end