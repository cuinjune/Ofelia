local objName = "ofSetup"
local log = pdLog()
local canvas = pdCanvas(this)
local clock = pdClock(this, "setup")
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
  ofWindow.addListener("setup", this, order)
  if ofWindow.exists then
    clock:delay(0)
  end
end

function ofelia.float(b)
  rendering = b ~= 0
end

function ofelia.order(f)
  order = f
  ofWindow.addListener("setup", this, order)
end

function ofelia.free()
  ofWindow.removeListener("setup", this)
end

function ofelia.setup()
  if not rendering then
    return
  end
  return nil
end