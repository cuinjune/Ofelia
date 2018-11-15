local objName = "of.setFullscreen"
local log = pd.Log()
local canvas = pd.Canvas(this)
local fullscreen = false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    fullscreen = args[1] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setFullscreen(fullscreen)
  return nil
end

function ofelia.fullscreen(b)
  fullscreen = b ~= 0
end