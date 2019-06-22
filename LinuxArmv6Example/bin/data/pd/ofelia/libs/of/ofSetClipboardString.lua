local log = pdLog("ofSetClipboardString")
local canvas = pdCanvas(this)
local str = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    str = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetClipboardString(str)
  return nil
end

function ofelia.str(s)
  str = s
end