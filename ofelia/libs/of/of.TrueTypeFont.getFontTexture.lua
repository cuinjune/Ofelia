local font = nil

function ofelia.bang()
  if type(font) ~= "userdata" then
    return of.Texture()
  end
  return font:getFontTexture()
end

function ofelia.font(p)
  font = p
end