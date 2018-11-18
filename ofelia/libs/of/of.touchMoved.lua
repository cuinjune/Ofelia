local objName = "of.touchMoved"
local log = pd.Log()
local canvas = pd.Canvas(this)
local order = 50

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    order = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
  pd.OFWindow.addListener("touchMoved", this, order)
end

function ofelia.order(f)
  order = f
  pd.OFWindow.addListener("touchMoved", this, order)
end

function ofelia.free()
  pd.OFWindow.removeListener("touchMoved", this)
end

function ofelia.touchMoved(e)
  return {e.type, e.x, e.y, e.id, e.time, e.numTouches, e.width, e.height, e.angle, e.minoraxis, e.majoraxis, e.pressure, e.xspeed, e.yspeed, e.xaccel, e.yaccel}
end