local objName = "of.getMousePressed"
local log = pd.Log()
local canvas = pd.Canvas(this)
local button = -1

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    button = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.getMousePressed(button)
end

function ofelia.button(i)
  button = i
end