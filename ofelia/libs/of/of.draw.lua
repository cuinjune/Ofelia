local objName = "of.draw"
local log = pd.Log()
local canvas = pd.Canvas(this)
local outlet = pd.Outlet(this)
local order = 50

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    order = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
  pd.OFWindow.addListener("draw", this, order)
end

function ofelia.order(f)
  order = f
  pd.OFWindow.addListener("draw", this, order)
end

function ofelia.free()
  pd.OFWindow.removeListener("draw", this)
end

function ofelia.draw()
  of.pushStyle()
  of.pushMatrix()
  outlet:outletBang(0)
  of.popMatrix()
  of.popStyle()
end