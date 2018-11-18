local objName = "of.mouseEntered"
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
  pd.OFWindow.addListener("mouseEntered", this, order)
end

function ofelia.order(f)
  order = f
  pd.OFWindow.addListener("mouseEntered", this, order)
end

function ofelia.free()
  pd.OFWindow.removeListener("mouseEntered", this)
end

function ofelia.mouseEntered(e)
  return {e.type, e.x, e.y, e.button, e.scrollX, e.scrollY, e.modifiers}
end