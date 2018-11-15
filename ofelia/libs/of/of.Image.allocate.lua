local objName = "of.Image.allocate"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local width, height, type = 0, 0, 2

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    width, height = args[1], args[2]
  elseif #args == 3 then
    width, height, type = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 or 3 creation arguments")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:allocate(width, height, type)
  return nil
end

function ofelia.width(i)
  width = i
end

function ofelia.height(i)
  height = i
end

function ofelia.type(i)
  type = i
end

function ofelia.image(p)
  image = p
end