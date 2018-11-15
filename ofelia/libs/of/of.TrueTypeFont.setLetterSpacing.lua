local objName = "of.TrueTypeFont.setLetterSpacing"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local spacing = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    spacing = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return nil
  end
  font:setLetterSpacing(spacing)
  return nil
end

function ofelia.spacing(f)
  spacing = f
end

function ofelia.font(p)
  font = p
end