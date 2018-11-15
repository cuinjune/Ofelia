local objName = "of.Image.setImageType"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local type = 2

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    type = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:setImageType(type)
  return nil
end

function ofelia.type(i)
  type = i
end

function ofelia.image(p)
  image = p
end