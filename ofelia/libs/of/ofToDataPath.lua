local objName = "ofToDataPath"
local log = pdLog()
local canvas = pdCanvas(this)
local path, absolute = "", false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    path = args[1]
  elseif #args == 2 then
    path, absolute = args[1], args[2] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  ofToDataPath(path, absolute)
  return nil
end

function ofelia.path(s)
  path = s
end

function ofelia.absolute(b)
  absolute = b ~= 0
end