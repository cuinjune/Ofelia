local objName = "of.TrueTypeFont.stringWidth"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local str = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    str = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return 0
  end
  return font:stringWidth(str)
end

function ofelia.str(s)
  str = s
end

function ofelia.font(p)
  font = p
end