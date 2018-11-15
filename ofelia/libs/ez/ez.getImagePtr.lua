local objName = "ez.getImagePtr"
local log = pd.Log()
local canvas = pd.Canvas(this)
local name = "$1"

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  local m = require("ez." .. name)
  if type(m.image) ~= "userdata" then
    return nil
  end
  return m.image
end

function ofelia.name(s)
  name = s
end