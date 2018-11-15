local objName = "of.TrueTypeFont.setSpaceSize"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
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
  if type(font) ~= "userdata" then
    return nil
  end
  font:setSpaceSize(size)
  return nil
end

function ofelia.size(f)
  size = f
end

function ofelia.font(p)
  font = p
end