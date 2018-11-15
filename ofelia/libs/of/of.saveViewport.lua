local objName = "of.saveViewport"
local log = pd.Log()
local canvas = pd.Canvas(this)
local filename = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    filename = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.saveViewport(filename)
  return nil
end

function ofelia.filename(s)
  filename = s
end