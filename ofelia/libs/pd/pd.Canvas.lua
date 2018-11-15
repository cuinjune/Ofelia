local objName = "pd.Canvas"
local log = pd.Log()
local canvas = pd.Canvas(this)
local canvas2 = pd.Canvas(this, 1)

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    canvas2 = pd.Canvas(this, args[1] + 1)
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.realizeDollar(s)
  return canvas2:realizeDollar(s)
end

function ofelia.getName()
  return canvas2:getName()
end

function ofelia.getIndex()
  return canvas2:getIndex()
end

function ofelia.getArgs()
  return canvas2:getArgs()
end

function ofelia.setArgs(av)
  if type(av) == "number" or type(av) == "string" then
    canvas2:setArgs({av})
  elseif type(av) == "table" then
    canvas2:setArgs(av)
  end
end

function ofelia.getPosition()
  return canvas2:getPosition()
end

function ofelia.setPosition(iv)
  canvas2:setPosition(iv[i], iv[2])
end

function ofelia.getDir()
  return canvas2:getDir()
end

function ofelia.remove()
  canvas2:remove()
end