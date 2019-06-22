local log = pdLog("ofSetFrameRate")
local canvas = pdCanvas(this)
local targetRate = 60

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    targetRate = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetFrameRate(targetRate)
  return nil
end

function ofelia.targetRate(i)
  targetRate = i
end