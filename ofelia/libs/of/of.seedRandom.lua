local objName = "of.seedRandom"
local log = pd.Log()
local canvas = pd.Canvas(this)
local val = nil

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    val = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if val == nil then
    of.seedRandom()
  else
    of.seedRandom(val)
  end
  return nil
end

function ofelia.val(i)
  val = i
end