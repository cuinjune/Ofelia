local objName = "ofMap"
local log = pdLog()
local canvas = pdCanvas(this)
local inputMin, inputMax, outputMin, outputMax, clamp = 0, 0, 0, 0, false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 4 then
    inputMin, inputMax, outputMin, outputMax = args[1], args[2], args[3], args[4]
  elseif #args == 5 then
    inputMin, inputMax, outputMin, outputMax, clamp = args[1], args[2], args[3], args[4], args[5] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 4 or 5 creation arguments")
  end
end

function ofelia.float(f)
  return ofMap(f, inputMin, inputMax, outputMin, outputMax, clamp)
end

function ofelia.inputMin(f)
  inputMin = f
end

function ofelia.inputMax(f)
  inputMax = f
end

function ofelia.outputMin(f)
  outputMin = f
end

function ofelia.outputMax(f)
  outputMax = f
end

function ofelia.clamp(b)
  clamp = b ~= 0
end