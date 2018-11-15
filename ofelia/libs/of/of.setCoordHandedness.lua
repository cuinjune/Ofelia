local objName = "of.setCoordHandedness"
local log = pd.Log()
local canvas = pd.Canvas(this)
local handedness = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    handedness = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setCoordHandedness(handedness)
  return nil
end

function ofelia.handedness(i)
  handedness = i
end