local objName = "of.hexToChar"
local log = pd.Log()
local canvas = pd.Canvas(this)
local charHexString = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    charHexString = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.hexToChar(charHexString)
end

function ofelia.charHexString(s)
  charHexString = s
end