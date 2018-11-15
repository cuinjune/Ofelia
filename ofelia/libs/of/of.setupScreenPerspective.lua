local objName = "of.setupScreenPerspective"
local log = pd.Log()
local canvas = pd.Canvas(this)
local width, height, fov, nearDist, farDist = 0, 0, 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 5 then
    width, height, fov, nearDist, farDist = args[1], args[2], args[3], args[4], args[5]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 5 creation arguments")
  end
end

function ofelia.bang()
  of.setupScreenPerspective(width, height, fov, nearDist, farDist)
  return nil
end

function ofelia.width(f)
  width = f
end

function ofelia.height(f)
  height = f
end

function ofelia.fov(f)
  fov = f
end

function ofelia.nearDist(f)
  nearDist = f
end

function ofelia.farDist(f)
  farDist = f
end