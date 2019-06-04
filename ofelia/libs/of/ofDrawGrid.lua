local objName = "ofDrawGrid"
local log = pdLog()
local canvas = pdCanvas(this)
local stepSize, numberOfSteps, labels, x, y, z = 1.25, 8, false, true, true, true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    stepSize = args[1]
  elseif #args == 2 then
    stepSize, numberOfSteps = args[1], args[2]
  elseif #args == 3 then
    stepSize, numberOfSteps, labels = args[1], args[2], args[3] ~= 0
  elseif #args == 6 then
    stepSize, numberOfSteps, labels, x, y, z = args[1], args[2], args[3] ~= 0, args[4] ~= 0, args[5] ~= 0, args[6] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 2, 3 or 6 creation arguments")
  end
end

function ofelia.bang()
  ofDrawGrid(stepSize, numberOfSteps, labels, x, y, z)
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

function ofelia.x(b)
  x = b ~= 0
end

function ofelia.y(b)
  y = b ~= 0
end

function ofelia.z(b)
  z = b ~= 0
end