local objName = "of.setupScreenOrtho"
local log = pd.Log()
local canvas = pd.Canvas(this)
local width, height, nearDist, farDist = 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 4 then
    width, height, nearDist, farDist = args[1], args[2], args[3], args[4]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 4 creation arguments")
  end
end

function ofelia.bang()
  of.setupScreenOrtho(width, height, nearDist, farDist)
  return nil
end

function ofelia.width(f)
  width = f
end

function ofelia.height(f)
  height = f
end

function ofelia.nearDist(f)
  nearDist = f
end

function ofelia.farDist(f)
  farDist = f
end