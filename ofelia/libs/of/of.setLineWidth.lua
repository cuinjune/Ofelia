local objName = "of.setLineWidth"
local log = pd.Log()
local canvas = pd.Canvas(this)
local lineWidth = 1

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    lineWidth = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setLineWidth(lineWidth)
  return nil
end

function ofelia.lineWidth(f)
  lineWidth = f
end