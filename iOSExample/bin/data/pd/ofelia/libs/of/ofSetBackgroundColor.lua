local log = pdLog("ofSetBackgroundColor")
local canvas = pdCanvas(this)
local color = ofColor()

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    color:set(args[1])
  elseif #args == 2 then
    color:set(args[1], args[2])
  elseif #args == 3 then
    color:set(args[1], args[2], args[3])
  elseif #args == 4 then
    color:set(args[1], args[2], args[3], args[4])
  elseif #args ~= 0 then
    log:error("requires 1, 2, 3 or 4 creation arguments")
  end
end

function ofelia.bang()
  ofSetBackgroundColor(color)
  return nil
end

function ofelia.r(i)
  color.r = i
end

function ofelia.g(i)
  color.g = i
end

function ofelia.b(i)
  color.b = i
end

function ofelia.a(i)
  color.a = i
end

function ofelia.color(iv)
  if type(iv) == "number" then
    color:set(iv)
  elseif #iv == 2 then
    color:set(iv[1], iv[2])
  elseif #iv == 3 then
    color:set(iv[1], iv[2], iv[3])
  elseif #iv == 4 then
    color:set(iv[1], iv[2], iv[3], iv[4])
  else
    log:error("'color' requires 1, 2, 3 or 4 arguments")
  end
end