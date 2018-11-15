local objName = "of.radToDeg"
local log = pd.Log()
local canvas = pd.Canvas(this)
local radians = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    radians = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.radToDeg(radians)
end

function ofelia.radians(f)
  radians = f
end