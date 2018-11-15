local objName = "of.toChar"
local log = pd.Log()
local canvas = pd.Canvas(this)
local charString = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    charString = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.toChar(charString)
end

function ofelia.charString(s)
  charString = s
end