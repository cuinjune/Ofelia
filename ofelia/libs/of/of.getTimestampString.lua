local objName = "of.getTimestampString"
local log = pd.Log()
local canvas = pd.Canvas(this)
local timestampFormat = "%Y-%m-%d-%H-%M-%S-%i"

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    timestampFormat = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.getTimestampString(timestampFormat)
end

function ofelia.timestampFormat(s)
  timestampFormat = s
end