local log = pdLog("ofGetFontStringMesh")
local canvas = pdCanvas(this)
local name = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.list(av)
  local m = require(name)
  if #av == 3 then
    return m.font:getStringMesh(av[1], av[2], av[3])
  elseif #av == 4 then
    return m.font:getStringMesh(av[1], av[2], av[3], av[4] ~= 0)
  else
    log:error("requires 3 or 4 creation argument")
  end
end

function ofelia.name(s)
  name = s
end