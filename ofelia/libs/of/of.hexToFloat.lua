local objName = "of.hexToFloat"
local log = pd.Log()
local canvas = pd.Canvas(this)
local floatHexString = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    floatHexString = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.hexToFloat(floatHexString)
end

function ofelia.floatHexString(s)
  floatHexString = s
end