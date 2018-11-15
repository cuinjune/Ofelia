local objName = "of.toInt64"
local log = pd.Log()
local canvas = pd.Canvas(this)
local intString = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    intString = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.toInt64(intString)
end

function ofelia.intString(s)
  intString = s
end