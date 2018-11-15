local objName = "of.getFixedStepForFps"
local log = pd.Log()
local canvas = pd.Canvas(this)
local fps = 60

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    fps = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.getFixedStepForFps(fps)
end

function ofelia.fps(i)
  fps = i
end