local objName = "of.toFloat"
local log = pd.Log()
local canvas = pd.Canvas(this)
local floatString = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    floatString = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.toFloat(floatString)
end

function ofelia.floatString(s)
  floatString = s
end