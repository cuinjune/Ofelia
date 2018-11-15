local objName = "of.map"
local log = pd.Log()
local canvas = pd.Canvas(this)
local value, inputMin, inputMax, outputMin, outputMax, clamp = 0, 0, 0, 0, 0, false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 5 then
    value, inputMin, inputMax, outputMin, outputMax = args[1], args[2], args[3], args[4], args[5]
  elseif #args == 6 then
    value, inputMin, inputMax, outputMin, outputMax, clamp = args[1], args[2], args[3], args[4], args[5], args[6] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 5 or 6 creation arguments")
  end
end

function ofelia.bang()
  return of.map(value, inputMin, inputMax, outputMin, outputMax, clamp)
end

function ofelia.value(f)
  value = f
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