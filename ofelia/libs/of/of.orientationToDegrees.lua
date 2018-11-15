local objName = "of.orientationToDegrees"
local log = pd.Log()
local canvas = pd.Canvas(this)
local orientation = 1

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    orientation = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.orientationToDegrees(orientation)
end

function ofelia.orientation(i)
  orientation = i
end