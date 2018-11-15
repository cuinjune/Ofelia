local font = nil

function ofelia.bang()
  if type(font) ~= "userdata" then
    return true
  end
  return font:hasFullCharacterSet()
end

function ofelia.font(p)
  font = p
end