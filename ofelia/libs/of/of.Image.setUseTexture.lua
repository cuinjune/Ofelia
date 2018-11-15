local objName = "of.Image.setUseTexture"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local bUse = false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    bUse = args[1] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:setUseTexture(bUse)
  return nil
end

function ofelia.bUse(b)
  bUse = b ~= 0
end

function ofelia.image(p)
  image = p
end