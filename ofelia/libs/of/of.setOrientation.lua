local objName = "of.setOrientation"
local log = pd.Log()
local canvas = pd.Canvas(this)
local orientation, vFlip = 1, true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    orientation = args[1]
  elseif #args == 2 then
    orientation, vFlip = args[1], args[2] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  of.setOrientation(orientation, vFlip)
  return nil
end

function ofelia.orientation(i)
  orientation = i
end

function ofelia.vFlip(b)
  vFlip = b ~= 0
end