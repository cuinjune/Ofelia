local log = pdLog("ofRandom")
local canvas = pdCanvas(this)
local min, max = 0, 1

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    max = args[1]
  elseif #args == 2 then
    min, max = args[1], args[2]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  return ofRandom(min, max)
end

function ofelia.min(f)
  min = f
end

function ofelia.max(f)
  max = f
end