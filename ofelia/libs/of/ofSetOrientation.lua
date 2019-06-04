local objName = "ofSetOrientation"
local log = pdLog()
local canvas = pdCanvas(this)
local orientation, vFlip = 1, true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    ofelia.orientation(args[1])
  elseif #args == 2 then
    ofelia.orientation(args[1])
    vFlip = args[2] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  ofSetOrientation(orientation, vFlip)
  return nil
end

function ofelia.orientation(a)
  if type(a) == "number" then
    orientation = a
  elseif a == "OF_ORIENTATION_DEFAULT" then
    orientation = 1
  elseif a == "OF_ORIENTATION_180" then
    orientation = 2
  elseif a == "OF_ORIENTATION_90_LEFT" then
    orientation = 3
  elseif a == "OF_ORIENTATION_90_RIGHT" then
    orientation = 4
  elseif a == "OF_ORIENTATION_UNKNOWN" then
    orientation = 5
  else
    log:error(objName .. " : unknown orientation : " .. a)
  end
end

function ofelia.vFlip(b)
  vFlip = b ~= 0
end