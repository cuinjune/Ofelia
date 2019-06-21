local log = pdLog("ofDrawAxis")
local canvas = pdCanvas(this)
local size = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    size = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofDrawAxis(size)
  return nil
end

function ofelia.size(f)
  size = f
end