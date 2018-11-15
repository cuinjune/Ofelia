local objName = "of.Image.getColor"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local x, y = 0, nil

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    x = args[1]
  elseif #args == 2 then
    x, y = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  if type(y) == "number" then
    return image:getColor(x, y)
  end
  return image:getColor(x)
end

function ofelia.x(i)
  x = i
end

function ofelia.y(i)
  y = i
end

function ofelia.image(p)
  image = p
end