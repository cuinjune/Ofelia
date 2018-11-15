local objName = "of.TrueTypeFont.setDirection"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local direction = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    direction = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return nil
  end
  font:setDirection(direction)
  return nil
end

function ofelia.direction(i)
  direction = i
end

function ofelia.font(p)
  font = p
end