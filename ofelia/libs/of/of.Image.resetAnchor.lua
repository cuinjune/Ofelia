local image = nil

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:resetAnchor()
  return nil
end

function ofelia.image(p)
  image = p
end