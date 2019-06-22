local log = pdLog("ofSetBackgroundAuto")
local canvas = pdCanvas(this)
local bAuto = true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    bAuto = args[1] ~= 0
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetBackgroundAuto(bAuto)
  return nil
end

function ofelia.bAuto(b)
  bAuto = b ~= 0
end