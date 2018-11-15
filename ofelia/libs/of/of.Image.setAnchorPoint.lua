local objName = "of.Image.setAnchorPoint"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local x, y = 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    x, y = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:setAnchorPoint(x, y)
  return nil
end

function ofelia.x(f)
  x = f
end

function ofelia.y(f)
  y = f
end

function ofelia.image(p)
  image = p
end