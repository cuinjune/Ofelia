local objName = "ofSetBoxResolution"
local log = pdLog()
local canvas = pdCanvas(this)
local resWidth, resHeight, resDepth = 1, 1, 1

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    resWidth, resHeight, resDepth = args[1], args[1], args[1]
  elseif #args == 3 then
    resWidth, resHeight, resDepth = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 3 creation arguments")
  end
end

function ofelia.bang()
  ofSetBoxResolution(resWidth, resHeight, resDepth)
  return nil
end

function ofelia.resWidth(i)
  resWidth = i
end

function ofelia.resHeight(i)
  resHeight = i
end

function ofelia.resDepth(i)
  resDepth = i
end

function ofelia.res(i)
  resWidth, resHeight, resDepth = i, i, i
end