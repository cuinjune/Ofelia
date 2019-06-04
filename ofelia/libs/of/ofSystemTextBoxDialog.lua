local objName = "ofSystemTextBoxDialog"
local log = pdLog()
local canvas = pdCanvas(this)
local text = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    text = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.symbol(s)
  return ofSystemTextBoxDialog(s, text)
end

function ofelia.text(s)
  text = s
end