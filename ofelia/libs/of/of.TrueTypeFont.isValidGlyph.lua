local objName = "of.TrueTypeFont.isValidGlyph"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local glyph = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    glyph = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return false
  end
  return font:isValidGlyph(glyph)
end

function ofelia.glyph(i)
  glyph = i
end

function ofelia.font(p)
  font = p
end