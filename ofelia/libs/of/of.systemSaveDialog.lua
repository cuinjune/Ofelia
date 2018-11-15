local objName = "of.systemSaveDialog"
local log = pd.Log()
local canvas = pd.Canvas(this)
local defaultName, messageName = "", ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    defaultName, messageName = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  return of.systemSaveDialog(defaultName, messageName)
end

function ofelia.defaultName(s)
  defaultName = s
end

function ofelia.messageName(s)
  messageName = s
end