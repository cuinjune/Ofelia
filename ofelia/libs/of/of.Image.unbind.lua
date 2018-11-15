local objName = "of.Image.unbind"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local textureLocation = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    textureLocation = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:unbind(textureLocation)
  return nil
end

function ofelia.textureLocation(i)
  textureLocation = i
end

function ofelia.image(p)
  image = p
end