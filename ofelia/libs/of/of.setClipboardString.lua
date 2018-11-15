local objName = "of.setClipboardString"
local log = pd.Log()
local canvas = pd.Canvas(this)
local str = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    str = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setClipboardString(str)
  return nil
end

function ofelia.str(s)
  str = s
end