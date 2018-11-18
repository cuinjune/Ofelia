local objName = "of.windowResized"
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
  pd.OFWindow.addListener("windowResized", this, order)
end

function ofelia.order(f)
  order = f
  pd.OFWindow.addListener("windowResized", this, order)
end

function ofelia.free()
  pd.OFWindow.removeListener("windowResized", this)
end

function ofelia.windowResized(e)
  return {e.width, e.height}
end