local objName = "of.setup"
local log = pd.Log()
local canvas = pd.Canvas(this)
local clock = pd.Clock(this, "setup")
local order = 50

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    order = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
  pd.OFWindow.addListener("setup", this, order)
  if pd.OFWindow.exists then
    clock:delay(0)
  end
end

function ofelia.order(f)
  order = f
  pd.OFWindow.addListener("setup", this, order)
end

function ofelia.free()
  pd.OFWindow.removeListener("setup", this)
end

function ofelia.setup()
  return nil
end