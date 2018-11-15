local objName = "of.drawAxis"
local log = pd.Log()
local canvas = pd.Canvas(this)
local size = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    size = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.drawAxis(size)
  return nil
end

function ofelia.size(f)
  size = f
end