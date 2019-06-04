local objName = "ofSetWindowShape"
local log = pdLog()
local canvas = pdCanvas(this)
local width, height = 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    width, height = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  ofSetWindowShape(width, height)
  return nil
end

function ofelia.width(i)
  width = i
end

function ofelia.height(i)
  height = i
end