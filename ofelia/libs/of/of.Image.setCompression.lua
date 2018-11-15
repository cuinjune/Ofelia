local objName = "of.Image.setCompression"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local compression = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    compression = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:setCompression(compression)
  return nil
end

function ofelia.compression(i)
  compression = i
end

function ofelia.image(p)
  image = p
end