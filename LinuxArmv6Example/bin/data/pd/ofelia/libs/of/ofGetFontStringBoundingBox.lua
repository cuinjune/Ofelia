local log = pdLog("ofGetFontStringBoundingBox")
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
    local rect = m.font:getStringBoundingBox(av[1], av[2], av[3])
    return {rect.x, rect.y, rect.width, rect.height}
  elseif #av == 4 then
    local rect = m.font:getStringBoundingBox(av[1], av[2], av[3], av[4] ~= 0)
    return {rect.x, rect.y, rect.width, rect.height}
  else
    log:error("requires 3 or 4 argument")
  end
end

function ofelia.name(s)
  name = s
end