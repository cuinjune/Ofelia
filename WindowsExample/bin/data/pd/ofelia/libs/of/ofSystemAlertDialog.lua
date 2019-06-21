local log = pdLog("ofSystemAlertDialog")
local canvas = pdCanvas(this)
local errorMessage = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    errorMessage = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSystemAlertDialog(errorMessage)
  return nil
end

function ofelia.errorMessage(s)
  errorMessage = s
end