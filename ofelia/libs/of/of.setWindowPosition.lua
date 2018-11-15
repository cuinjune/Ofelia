local objName = "of.setWindowPosition"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x, y = 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    x, y = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  of.setWindowPosition(x, y)
  return nil
end

function ofelia.x(i)
  x = i
end

function ofelia.y(i)
  y = i
end