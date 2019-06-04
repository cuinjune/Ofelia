local objName = "ofSetBackgroundColorHex"
local log = pdLog()
local canvas = pdCanvas(this)
local hexColor, alpha = ofHexToInt("0xffffff"), 255

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    hexColor = ofHexToInt(args[1])
  elseif #args == 2 then
    hexColor, alpha = ofHexToInt(args[1]), args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  ofSetBackgroundColorHex(hexColor, alpha)
  return nil
end

function ofelia.hexColor(s)
  hexColor = ofHexToInt(s)
end

function ofelia.alpha(i)
  alpha = i
end