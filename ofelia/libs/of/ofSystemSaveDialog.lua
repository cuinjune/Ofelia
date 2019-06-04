local objName = "ofSystemSaveDialog"
local log = pdLog()
local canvas = pdCanvas(this)
local messageName = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    messageName = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.symbol(s)
  return ofSystemSaveDialog(s, messageName)
end

function ofelia.messageName(s)
  messageName = s
end