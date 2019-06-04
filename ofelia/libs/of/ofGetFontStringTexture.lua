local objName = "ofGetFontStringTexture"
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

function ofelia.symbol(s)
  local m = require(name)
  return m.font:getStringTexture(s)
end

function ofelia.list(av)
  local m = require(name)
  return m.font:getStringTexture(av[1], av[2] ~= 0)
end

function ofelia.name(s)
  name = s
end