local objName = "of.enableBlendMode"
local log = pd.Log()
local canvas = pd.Canvas(this)
local mode = 1

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    mode = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.enableBlendMode(mode)
  return nil
end

function ofelia.mode(i)
  mode = i
end