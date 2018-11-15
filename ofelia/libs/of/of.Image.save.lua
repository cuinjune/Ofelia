local objName = "of.Image.save"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local fileName, compressionLevel = "", 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    fileName = args[1]
  elseif #args == 2 then
    fileName, compressionLevel = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return false
  end
  return image:save(fileName, compressionLevel)
end

function ofelia.fileName(s)
  fileName = s
end

function ofelia.compressionLevel(i)
  compressionLevel = i
end

function ofelia.image(p)
  image = p
end