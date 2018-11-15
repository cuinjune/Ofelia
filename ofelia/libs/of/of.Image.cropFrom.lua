local objName = "of.Image.cropFrom"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local otherImage, x, y, width, height = nil, 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 4 then
    x, y, width, height = args[1], args[2], args[3], args[4]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 4 creation arguments")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  if type(otherImage) ~= "userdata" then
    return nil
  end
  image:cropFrom(otherImage, x, y, width, height)
  return nil
end

function ofelia.otherImage(p)
  otherImage = p
end

function ofelia.x(i)
  x = i
end

function ofelia.y(i)
  y = i
end

function ofelia.width(i)
  width = i
end

function ofelia.height(i)
  height = i
end

function ofelia.image(p)
  image = p
end