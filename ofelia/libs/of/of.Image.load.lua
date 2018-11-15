local objName = "of.Image.load"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local fileName, settings = "", of.ImageLoadSettings()

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    fileName = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return false
  end
  return image:load(fileName, settings)
end

function ofelia.fileName(s)
  fileName = s
end

function ofelia.settings(p)
  settings = p
end

function ofelia.image(p)
  image = p
end