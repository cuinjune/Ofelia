local image = nil

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  return image:getTexture()
end

function ofelia.image(p)
  image = p
end