local log = pdLog("ofSetVerticalSync")
local canvas = pdCanvas(this)
local bSync = true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    bSync = args[1] ~= 0
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetVerticalSync(bSync)
  return nil
end

function ofelia.bSync(b)
  bSync = b ~= 0
end