local objName = "of.setTimeModeFiltered"
local log = pd.Log()
local canvas = pd.Canvas(this)
local alpha = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    alpha = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setTimeModeFiltered(alpha)
  return nil
end

function ofelia.alpha(f)
  alpha = f
end