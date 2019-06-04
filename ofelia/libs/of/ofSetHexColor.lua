local objName = "ofSetHexColor"
local log = pdLog()
local canvas = pdCanvas(this)
local hexColor = ofHexToInt("0xffffff")

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    hexColor = ofHexToInt(args[1])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetHexColor(hexColor)
  return nil
end

function ofelia.hexColor(s)
  hexColor = ofHexToInt(s)
end