local objName = "of.toHex"
local log = pd.Log()
local canvas = pd.Canvas(this)
local value = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    value = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.toHex(value)
end

function ofelia.value(s)
  value = s
end