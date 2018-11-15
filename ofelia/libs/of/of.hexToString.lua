local objName = "of.hexToString"
local log = pd.Log()
local canvas = pd.Canvas(this)
local stringHexString = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    stringHexString = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.hexToString(stringHexString)
end

function ofelia.stringHexString(s)
  stringHexString = s
end