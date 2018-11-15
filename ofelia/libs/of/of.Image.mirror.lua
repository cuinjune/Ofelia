local objName = "of.Image.mirror"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local vertical, horizontal = false, false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    vertical, horizontal = args[1] ~= 0, args[2] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:mirror(vertical, horizontal)
  return nil
end

function ofelia.vertical(b)
  vertical = b ~= 0
end

function ofelia.horizontal(b)
  horizontal = b ~= 0
end

function ofelia.image(p)
  image = p
end