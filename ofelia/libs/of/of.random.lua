local objName = "of.random"
local log = pd.Log()
local canvas = pd.Canvas(this)
local min, max = 0, 1

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    max = args[1]
  elseif #args == 2 then
    min, max = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.random(min, max)
end

function ofelia.min(f)
  min = f
end

function ofelia.max(f)
  max = f
end