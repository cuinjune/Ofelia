local log = pdLog("ofGetFboMaxColorAttachments")
local canvas = pdCanvas(this)
local name = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  return ofFbo.maxColorAttachments()
end

function ofelia.name(s)
  name = s
end