local font = nil

function ofelia.bang()
  if type(font) ~= "userdata" then
    return of.Rectangle()
  end
  return font:getGlyphBBox()
end

function ofelia.font(p)
  font = p
end