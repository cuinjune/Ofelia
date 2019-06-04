local objName = "ofBackgroundGradient"
local log = pdLog()
local canvas = pdCanvas(this)
local c1, c2, mode = ofColor(), ofColor(), OF_GRADIENT_CIRCULAR

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    c1:set(args[1])
    c2:set(args[2])
  elseif #args == 3 then
    c1:set(args[1])
    c2:set(args[2])
    ofelia.mode(args[3])
  elseif #args == 4 then
    c1:set(args[1], args[2])
    c2:set(args[3], args[4])
  elseif #args == 5 then
    c1:set(args[1], args[2])
    c2:set(args[3], args[4])
    ofelia.mode(args[5])
  elseif #args == 6 then
    c1:set(args[1], args[2], args[3])
    c2:set(args[4], args[5], args[6])
  elseif #args == 7 then
    c1:set(args[1], args[2], args[3])
    c2:set(args[4], args[5], args[6])
    ofelia.mode(args[7])
  elseif #args == 8 then
    c1:set(args[1], args[2], args[3], args[4])
    c2:set(args[5], args[6], args[7], args[8])
  elseif #args == 9 then
    c1:set(args[1], args[2], args[3], args[4])
    c2:set(args[5], args[6], args[7], args[8])
    ofelia.mode(args[9])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2, 3, 4, 5, 6, 7, 8 or 9 creation arguments")
  end
end

function ofelia.bang()
  ofBackgroundGradient(c1, c2, mode)
  return nil
end

function ofelia.c1(iv)
  if type(iv) == "number" then
    c1:set(iv)
  elseif #iv == 2 then
    c1:set(iv[1], iv[2])
  elseif #iv == 3 then
    c1:set(iv[1], iv[2], iv[3])
  elseif #iv == 4 then
    c1:set(iv[1], iv[2], iv[3], iv[4])
  else
    log:error(objName .. " : 'c1' requires 1, 2, 3 or 4 arguments")
  end
end

function ofelia.c2(iv)
  if type(iv) == "number" then
    c2:set(iv)
  elseif #iv == 2 then
    c2:set(iv[1], iv[2])
  elseif #iv == 3 then
    c2:set(iv[1], iv[2], iv[3])
  elseif #iv == 4 then
    c2:set(iv[1], iv[2], iv[3], iv[4])
  else
    log:error(objName .. " : 'c2' requires 1, 2, 3 or 4 arguments")
  end
end

function ofelia.mode(a)
  if type(a) == "number" then
    mode = a
  elseif a == "OF_GRADIENT_LINEAR" then
    mode = OF_GRADIENT_LINEAR
  elseif a == "OF_GRADIENT_CIRCULAR" then
    mode = OF_GRADIENT_CIRCULAR
  elseif a == "OF_GRADIENT_BAR" then
    mode = OF_GRADIENT_BAR
  else
    log:error(objName .. " : unknown mode : " .. a)
  end
end