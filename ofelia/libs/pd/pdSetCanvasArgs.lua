local objName = "pdSetCanvasArgs"
local log = pdLog()
local canvas = pdCanvas(this)
local canvas2 = pdCanvas(this, 1)

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    canvas2 = pdCanvas(this, args[1] + 1)
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.float(f)
  canvas2:setArgs({f})
  return nil
end

function ofelia.symbol(s)
  canvas2:setArgs({s})
  return nil
end

function ofelia.list(av)
  canvas2:setArgs(av)
  return nil
end