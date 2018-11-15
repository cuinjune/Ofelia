local image = nil

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  return image:getWidth()
end

function ofelia.image(p)
  image = p
end