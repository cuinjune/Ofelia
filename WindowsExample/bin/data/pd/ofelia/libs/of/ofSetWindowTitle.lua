local log = pdLog("ofSetWindowTitle")
local canvas = pdCanvas(this)
local title = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    title = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetWindowTitle(title)
  return nil
end

function ofelia.title(s)
  title = s
end