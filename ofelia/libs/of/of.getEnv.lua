local objName = "of.getEnv"
local log = pd.Log()
local canvas = pd.Canvas(this)
local var = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    var = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.getEnv(var)
end

function ofelia.var(s)
  var = s
end