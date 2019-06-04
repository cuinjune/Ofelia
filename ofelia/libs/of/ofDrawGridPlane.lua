local objName = "ofDrawGridPlane"
local log = pdLog()
local canvas = pdCanvas(this)
local stepSize, numberOfSteps, labels = 1.25, 8, false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    stepSize = args[1]
  elseif #args == 2 then
    stepSize, numberOfSteps = args[1], args[2]
  elseif #args == 3 then
    stepSize, numberOfSteps, labels = args[1], args[2], args[3] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 2 or 3 creation arguments")
  end
end

function ofelia.bang()
  ofDrawGridPlane(stepSize, numberOfSteps, labels)
  return nil
end

function ofelia.stepSize(f)
  stepSize = f
end

function ofelia.numberOfSteps(i)
  numberOfSteps = i
end

function ofelia.labels(b)
  labels = b ~= 0
end