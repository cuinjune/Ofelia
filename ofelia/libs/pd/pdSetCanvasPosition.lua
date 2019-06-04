local objName = "pdSetCanvasPosition"
local log = pdLog()
local canvas = pdCanvas(this)
local canvas2 = pdCanvas(this, 0)

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    canvas2 = pdCanvas(this, args[1] + 0)
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.list(iv)
  canvas2:setPosition(iv[1], iv[2])
  return nil
end