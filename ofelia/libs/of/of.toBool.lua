local objName = "of.toBool"
local log = pd.Log()
local canvas = pd.Canvas(this)
local boolString = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    boolString = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.toBool(boolString)
end

function ofelia.boolString(s)
  boolString = s
end