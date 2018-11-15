local objName = "of.Image.setColor"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local x, y, color = nil, nil, of.Color(255)

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    x = args[1]
  elseif #args == 2 then
    x, y = args[1], args[2]
  elseif #args == 4 then
    color:set(args[1], args[2], args[3], args[4])
  elseif #args == 5 then
    x = args[1]
    color:set(args[2], args[3], args[4], args[5])
  elseif #args == 6 then
    x, y = args[1], args[2]
    color:set(args[3], args[4], args[5], args[6])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 2, 4, 5 or 6 creation arguments")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  if type(x) ~= "number" then
    image:setColor(color)
  elseif type(y) ~= "number" then
    image:setColor(x, color)
  else
    image:setColor(x, y, color)
  end
  return nil
end

function ofelia.x(i)
  x = i
end

function ofelia.y(i)
  y = i
end

function ofelia.color(p)
  color = p
end

function ofelia.image(p)
  image = p
end