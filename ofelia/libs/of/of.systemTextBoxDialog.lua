local objName = "of.systemTextBoxDialog"
local log = pd.Log()
local canvas = pd.Canvas(this)
local question, text = "", ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    question = args[1]
  elseif #args == 2 then
    question, text = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  return of.systemTextBoxDialog(question, text)
end

function ofelia.question(s)
  question = s
end

function ofelia.text(s)
  text = s
end