local font = nil

function ofelia.bang()
  if type(font) ~= "userdata" then
    return false
  end
  return font:isLoaded()
end

function ofelia.font(p)
  font = p
end