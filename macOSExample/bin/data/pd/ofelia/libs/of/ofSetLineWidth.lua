local log = pdLog("ofSetLineWidth")
local canvas = pdCanvas(this)
local lineWidth = 1

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    lineWidth = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetLineWidth(lineWidth)
  return nil
end

function ofelia.lineWidth(f)
  lineWidth = f
end