local objName = "ofLerp"
local log = pdLog()
local canvas = pdCanvas(this)
local start, stop = 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    start, stop = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.float(f)
  return ofLerp(start, stop, f)
end

function ofelia.start(f)
  start = f
end

function ofelia.stop(f)
  stop = f
end