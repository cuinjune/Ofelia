local objName = "of.nextPow2"
local log = pd.Log()
local canvas = pd.Canvas(this)
local a = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    a = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.nextPow2(a)
end

function ofelia.a(i)
  a = i
end