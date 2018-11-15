local objName = "of.hexToInt"
local log = pd.Log()
local canvas = pd.Canvas(this)
local intHexString = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    intHexString = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.hexToInt(intHexString)
end

function ofelia.intHexString(s)
  intHexString = s
end