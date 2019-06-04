local objName = "ofToUpper"
local log = pdLog()
local canvas = pdCanvas(this)
local locale = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    locale = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.symbol(s)
  return ofToUpper(s, locale)
end

function ofelia.locale(s)
  locale = s
end