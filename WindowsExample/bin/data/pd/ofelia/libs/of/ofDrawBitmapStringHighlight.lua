local log = pdLog("ofDrawBitmapStringHighlight")
local canvas = pdCanvas(this)
local text, x, y, background, foreground = "", 0, 0, ofColor(0), ofColor(255)

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    text, x, y = args[1], args[2], args[3]
  elseif #args == 9 then
    text, x, y = args[1], args[2], args[3]
    background:set(args[4], args[5], args[6])
    foreground:set(args[7], args[8], args[9])
  elseif #args == 11 then
    text, x, y = args[1], args[2], args[3]
    background:set(args[4], args[5], args[6], args[7])
    foreground:set(args[8], args[9], args[10], args[11])
  elseif #args ~= 0 then
    log:error("requires 3, 9 or 11 creation arguments")
  end
end

function ofelia.bang()
  ofDrawBitmapStringHighlight(text, x, y, background, foreground)
  return nil
end

function ofelia.text(s)
  text = s
end

function ofelia.x(i)
  x = i
end

function ofelia.y(i)
  y = i
end

function ofelia.background(iv)
  if type(iv) == "number" then
    background:set(iv)
  elseif #iv == 2 then
    background:set(iv[1], iv[2])
  elseif #iv == 3 then
    background:set(iv[1], iv[2], iv[3])
  elseif #iv == 4 then
    background:set(iv[1], iv[2], iv[3], iv[4])
  else
    log:error("'background' requires 1, 2, 3 or 4 arguments")
  end
end

function ofelia.foreground(iv)
  if type(iv) == "number" then
    foreground:set(iv)
  elseif #iv == 2 then
    foreground:set(iv[1], iv[2])
  elseif #iv == 3 then
    foreground:set(iv[1], iv[2], iv[3])
  elseif #iv == 4 then
    foreground:set(iv[1], iv[2], iv[3], iv[4])
  else
    log:error("'foreground' requires 1, 2, 3 or 4 arguments")
  end
end