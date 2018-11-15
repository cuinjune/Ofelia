local objName = "of.touchCancelled"
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
  pd.OFWindow.addListener("touchCancelled", this, order)
end

function ofelia.order(f)
  order = f
  pd.OFWindow.addListener("touchCancelled", this, order)
end

function ofelia.free()
  pd.OFWindow.removeListener("touchCancelled", this)
end

function ofelia.touchCancelled(e)
  return e
end