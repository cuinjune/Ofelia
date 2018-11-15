local objName = "of.getKeyPressed"
local log = pd.Log()
local canvas = pd.Canvas(this)
local key = -1

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    key = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.getKeyPressed(key)
end

function ofelia.key(i)
  key = i
end