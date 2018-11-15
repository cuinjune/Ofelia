local font = nil

function ofelia.bang()
  if type(font) ~= "userdata" then
    return true
  end
  return font:isAntiAliased()
end

function ofelia.font(p)
  font = p
end