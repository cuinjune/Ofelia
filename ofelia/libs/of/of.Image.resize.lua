local objName = "of.Image.resize"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local newWidth, newHeight = 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    newWidth, newHeight = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:resize(newWidth, newHeight)
  return nil
end

function ofelia.newWidth(i)
  newWidth = i
end

function ofelia.newHeight(i)
  newHeight = i
end

function ofelia.image(p)
  image = p
end