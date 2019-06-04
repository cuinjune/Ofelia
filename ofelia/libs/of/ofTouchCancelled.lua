local objName = "ofTouchCancelled"
local log = pdLog()
local canvas = pdCanvas(this)
local order, rendering = 50, true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    order = args[1]
  elseif #args == 2 then
    order, rendering = args[1], args[2] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
  ofWindow.addListener("touchCancelled", this, order)
end

function ofelia.float(b)
  rendering = b ~= 0
end

function ofelia.order(f)
  order = f
  ofWindow.addListener("touchCancelled", this, order)
end

function ofelia.free()
  ofWindow.removeListener("touchCancelled", this)
end

function ofelia.touchCancelled(e)
  if not rendering then
    return
  end
  return {e.type, e.x, e.y, e.id, e.time, e.numTouches, e.width, e.height, e.angle, e.minoraxis, e.majoraxis, e.pressure, e.xspeed, e.yspeed, e.xaccel, e.yaccel}
end