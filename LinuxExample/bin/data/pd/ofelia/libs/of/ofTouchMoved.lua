local log = pdLog("ofTouchMoved")
local canvas = pdCanvas(this)
local order, enabled = 50, true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    order = args[1]
  elseif #args == 2 then
    order, enabled = args[1], args[2] ~= 0
  elseif #args ~= 0 then
    log:error("requires 1 or 2 creation arguments")
  end
  ofWindow.addListener("touchMoved", this, order)
end

function ofelia.float(b)
  enabled = b ~= 0
end

function ofelia.order(f)
  order = f
  ofWindow.addListener("touchMoved", this, order)
end

function ofelia.free()
  ofWindow.removeListener("touchMoved", this)
end

function ofelia.touchMoved(e)
  if not enabled then
    return
  end
  return {e.type, e.x, e.y, e.id, e.time, e.numTouches, e.width, e.height, e.angle, e.minoraxis, e.majoraxis, e.pressure, e.xspeed, e.yspeed, e.xaccel, e.yaccel}
end