local objName = "of.system"
local log = pd.Log()
local canvas = pd.Canvas(this)
local command = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    command = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.system(command)
end

function ofelia.command(s)
  command = s
end