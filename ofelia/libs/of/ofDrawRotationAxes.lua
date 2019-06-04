local objName = "ofDrawRotationAxes"
local log = pdLog()
local canvas = pdCanvas(this)
local radius, stripWidth, circleRes = 0, 10, 60

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    radius = args[1]
  elseif #args == 2 then
    radius, stripWidth = args[1], args[2]
  elseif #args == 3 then
    radius, stripWidth, circleRes = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 2 or 3 creation arguments")
  end
end

function ofelia.bang()
  ofDrawRotationAxes(radius, stripWidth, circleRes)
  return nil
end

function ofelia.radius(f)
  radius = f
end

function ofelia.stripWidth(f)
  stripWidth = f
end

function ofelia.circleRes(i)
  circleRes = i
end