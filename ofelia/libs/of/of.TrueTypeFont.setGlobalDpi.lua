local objName = "of.TrueTypeFont.setGlobalDpi"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local newDpi = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    newDpi = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return nil
  end
  font:setGlobalDpi(newDpi)
  return nil
end

function ofelia.newDpi(i)
  newDpi = i
end

function ofelia.font(p)
  font = p
end