local log = pdLog("ofDrawFbo")
local canvas = pdCanvas(this)
local name, x, y, width, height = "", 0, 0, nil, nil

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args == 3 then
    name, x, y = args[1], args[2], args[3]
  elseif #args == 5 then
    name, x, y, width, height = args[1], args[2], args[3], args[4], args[5]
  elseif #args ~= 0 then
    log:error("requires 1, 3 or 5 creation arguments")
  end
end

function ofelia.bang()
  local m = require(name)
  if type(width) == "nil" or type(height) == "nil" then
    m.fbo:draw(x, y)
    return nil
  end
  m.fbo:draw(x, y, width, height)
  return nil
end

function ofelia.name(s)
  name = s
end

function ofelia.x(f)
  x = f
end

function ofelia.y(f)
  y = f
end

function ofelia.width(f)
  width = f
end

function ofelia.height(f)
  height = f
end