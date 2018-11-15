local objName = "of.clamp"
local log = pd.Log()
local canvas = pd.Canvas(this)
local value, min, max = 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    value, min, max = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 3 creation arguments")
  end
end

function ofelia.bang()
  return of.clamp(value, min, max)
end

function ofelia.value(f)
  value = f
end

function ofelia.min(f)
  min = f
end

function ofelia.max(f)
  max = f
end