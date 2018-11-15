local objName = "of.lerp"
local log = pd.Log()
local canvas = pd.Canvas(this)
local start, stop, amt = 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    start, stop, amt = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 3 creation arguments")
  end
end

function ofelia.bang()
  return of.lerp(start, stop, amt)
end

function ofelia.start(f)
  start = f
end

function ofelia.stop(f)
  stop = f
end

function ofelia.amt(f)
  amt = f
end