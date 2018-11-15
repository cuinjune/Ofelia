local objName = "of.wrap"
local log = pd.Log()
local canvas = pd.Canvas(this)
local value, from, to = 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    value, from, to = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 3 creation arguments")
  end
end

function ofelia.bang()
  return of.wrap(value, from, to)
end

function ofelia.value(f)
  value = f
end

function ofelia.from(f)
  from = f
end

function ofelia.to(f)
  to = f
end