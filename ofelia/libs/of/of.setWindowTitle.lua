local objName = "of.setWindowTitle"
local log = pd.Log()
local canvas = pd.Canvas(this)
local title = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    title = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setWindowTitle(title)
  return nil
end

function ofelia.title(s)
  title = s
end