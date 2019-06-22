local log = pdLog("ofBindImage")
local canvas = pdCanvas(this)
local name, textureLocation = "", 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args == 2 then
    name, textureLocation = args[1], args[2]
  elseif #args ~= 0 then
    log:error("requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  local m = require(name)
  m.image:bind(textureLocation)
  return nil
end

function ofelia.name(s)
  name = s
end

function ofelia.textureLocation(i)
  textureLocation = i
end