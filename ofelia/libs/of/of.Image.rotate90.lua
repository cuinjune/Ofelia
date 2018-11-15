local objName = "of.Image.rotate90"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local rotation = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    rotation = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:rotate90(rotation)
  return nil
end

function ofelia.rotation(i)
  rotation = i
end

function ofelia.image(p)
  image = p
end