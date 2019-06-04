local objName = "ofSetRectMode"
local log = pdLog()
local canvas = pdCanvas(this)
local mode = OF_RECTMODE_CORNER

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    ofelia.mode(args[1])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetRectMode(mode)
  return nil
end

function ofelia.mode(a)
  if type(a) == "number" then
    mode = a
  elseif a == "OF_RECTMODE_CORNER" then
    mode = OF_RECTMODE_CORNER
  elseif a == "OF_RECTMODE_CENTER" then
    mode = OF_RECTMODE_CENTER
  else
    log:error(objName .. " : unknown mode : " .. a)
  end
end