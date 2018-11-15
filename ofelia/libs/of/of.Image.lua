local objName = "of.Image"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local fileName = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    fileName = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
  pd.OFWindow.addListener("exit", this)
end

function ofelia.clear()
  if type(image) == "userdata" then
    image:clear()
  end
  image = nil
end

function ofelia.free()
  ofelia.clear()
  pd.OFWindow.removeListener("exit", this)
end

function ofelia.exit()
  ofelia.clear()
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    image = of.Image(fileName)
  end
  return image
end