local image = nil

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:clear()
  return nil
end

function ofelia.image(p)
  image = p
end