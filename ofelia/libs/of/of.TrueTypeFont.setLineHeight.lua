local objName = "of.TrueTypeFont.setLineHeight"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local height = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    height = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return nil
  end
  font:setLineHeight(height)
  return nil
end

function ofelia.height(f)
  height = f
end

function ofelia.font(p)
  font = p
end