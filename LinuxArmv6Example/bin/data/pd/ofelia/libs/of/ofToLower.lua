local log = pdLog("ofToLower")
local canvas = pdCanvas(this)
local locale = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    locale = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.symbol(s)
  return ofToLower(s, locale)
end

function ofelia.locale(s)
  locale = s
end