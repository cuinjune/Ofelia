local log = pdLog("ofGetFontGlyphBBox")
local canvas = pdCanvas(this)
local name = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  local m = require(name)
  local rect = m.font:getGlyphBBox()
  return {rect.x, rect.y, rect.width, rect.height}
end

function ofelia.name(s)
  name = s
end