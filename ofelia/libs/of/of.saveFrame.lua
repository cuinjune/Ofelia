local objName = "of.saveFrame"
local log = pd.Log()
local canvas = pd.Canvas(this)
local bUseViewport = false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    bUseViewport = args[1] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.saveFrame(bUseViewport)
  return nil
end

function ofelia.bUseViewport(b)
  bUseViewport = b ~= 0
end