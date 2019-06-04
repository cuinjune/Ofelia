local objName = "ofViewport"
local log = pdLog()
local canvas = pdCanvas(this)
local x, y, width, height, invertY = 0, 0, -1, -1, true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 4 then
    x, y, width, height  = args[1], args[2], args[3], args[4]
  elseif #args == 5 then
    x, y, width, height, invertY = args[1], args[2], args[3], args[4], args[5] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 4 or 5 creation arguments")
  end
end

function ofelia.bang()
  ofViewport(x, y, width, height, invertY)
  return nil
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

function ofelia.invertY(b)
  invertY = b ~= 0
end

function ofelia.rectangle(fv)
  x, y, width, height = fv[1], fv[2], fv[3], fv[4]
end