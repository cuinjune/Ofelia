local objName = "of.wrapRadians"
local log = pd.Log()
local canvas = pd.Canvas(this)
local angle, from, to = 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    angle, from, to = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 3 creation arguments")
  end
end

function ofelia.bang()
  return of.wrapRadians(angle, from, to)
end

function ofelia.angle(f)
  angle = f
end

function ofelia.from(f)
  from = f
end

function ofelia.to(f)
  to = f
end