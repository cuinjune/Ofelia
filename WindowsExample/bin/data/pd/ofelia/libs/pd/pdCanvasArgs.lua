local log = pdLog("pdCanvasArgs")
local canvas = pdCanvas(this)
local canvas2 = pdCanvas(this, 1)

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    canvas2 = pdCanvas(this, args[1] + 1)
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  return canvas2:getArgs()
end

function ofelia.float(f)
  canvas2:setArgs({f})
end

function ofelia.symbol(s)
  canvas2:setArgs({s})
end

function ofelia.list(av)
  canvas2:setArgs(av)
end