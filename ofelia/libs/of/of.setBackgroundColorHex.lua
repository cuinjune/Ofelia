local objName = "of.setBackgroundColorHex"
local log = pd.Log()
local canvas = pd.Canvas(this)
local hexColor, alpha = of.hexToInt("0xc8c8c8"), 255

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    hexColor = of.hexToInt(args[1])
  elseif #args == 2 then
    hexColor, alpha = of.hexToInt(args[1]), args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  of.setBackgroundColorHex(hexColor, alpha)
  return nil
end

function ofelia.hexColor(s)
  hexColor = of.hexToInt(s)
end

function ofelia.alpha(i)
  alpha = i
end