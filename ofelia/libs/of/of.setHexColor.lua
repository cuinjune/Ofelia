local objName = "of.setHexColor"
local log = pd.Log()
local canvas = pd.Canvas(this)
local hexColor = of.hexToInt("0xffffff")

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    hexColor = of.hexToInt(args[1])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setHexColor(hexColor)
  return nil
end

function ofelia.hexColor(s)
  hexColor = of.hexToInt(s)
end