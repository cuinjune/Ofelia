local objName = "of.toDouble"
local log = pd.Log()
local canvas = pd.Canvas(this)
local doubleString = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    doubleString = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.toDouble(doubleString)
end

function ofelia.doubleString(s)
  doubleString = s
end