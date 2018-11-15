local image = nil
local pixels = nil

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  if type(pixels) ~= "userdata" then
    return nil
  end
  image:setFromPixels(pixels)
  return nil
end

function ofelia.pixels(p)
  pixels = p
end

function ofelia.image(p)
  image = p
end