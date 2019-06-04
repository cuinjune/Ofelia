local objName = "ofGetImageColor"
local log = pdLog()
local canvas = pdCanvas(this)
local name = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.float(i)
  local m = require(name)
  local color = m.image:getColor(i)
  return {color.r, color.g, color.b, color.a}
end

function ofelia.list(iv)
  local m = require(name)
  local color = m.image:getColor(iv[1], iv[2])
  return {color.r, color.g, color.b, color.a}
end

function ofelia.name(s)
  name = s
end