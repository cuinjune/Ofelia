local objName = "of.noise"
local log = pd.Log()
local canvas = pd.Canvas(this)
local x, y, z, w = 0, nil, nil, nil

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    x = args[1]
  elseif #args == 2 then
    x, y = args[1], args[2]
  elseif #args == 3 then
    x, y, z = args[1], args[2], args[3]
  elseif #args == 4 then
    x, y, z, w = args[1], args[2], args[3], args[4]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 2, 3 or 4 creation arguments")
  end
end

function ofelia.bang()
  if y == nil then
    return of.noise(x)
  elseif z == nil then
    return of.noise(x, y)
  elseif w == nil then
    return of.noise(x, y, z)
  else
    return of.noise(x, y, z, w)
  end
end

function ofelia.x(f)
  x = f
end

function ofelia.y(f)
  y = f
end

function ofelia.z(f)
  z = f
end

function ofelia.w(f)
  w = f
end

function ofelia.pos(p)
  x, y, z = p.x, p.y, p.z
end