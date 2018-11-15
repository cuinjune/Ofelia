local objName = "of.systemAlertDialog"
local log = pd.Log()
local canvas = pd.Canvas(this)
local errorMessage = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    errorMessage = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.systemAlertDialog(errorMessage)
  return nil
end

function ofelia.errorMessage(s)
  errorMessage = s
end