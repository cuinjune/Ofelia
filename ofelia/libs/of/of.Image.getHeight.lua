local image = nil

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  return image:getHeight()
end

function ofelia.image(p)
  image = p
end