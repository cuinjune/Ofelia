local objName = "of.Image.drawSubsection"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local x, y, z, width, height, sx, sy, swidth, sheight = 0, 0, 0, 0, 0, 0, 0, nil, nil

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 6 then
    x, y, width, height, sx, sy = args[1], args[2], args[3], args[4], args[5], args[6]
  elseif #args == 7 then
    x, y, z, width, height, sx, sy = args[1], args[2], args[3], args[4], args[5], args[6], args[7]
  elseif #args == 8 then
    x, y, width, height, sx, sy, swidth, sheight = args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]
  elseif #args == 9 then
    x, y, z, width, height, sx, sy, swidth, sheight = args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 6, 7, 8 or 9 creation arguments")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  if type(swidth) == "nil" or type(sheight) == "nil" then
    image:drawSubsection(x, y, z, width, height, sx, sy)
    return nil
  end
  image:drawSubsection(x, y, z, width, height, sx, sy, swidth, sheight)
  return nil
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

function ofelia.width(f)
  width = f
end

function ofelia.height(f)
  height = f
end

function ofelia.sx(f)
  sx = f
end

function ofelia.sy(f)
  sy = f
end

function ofelia.swidth(f)
  swidth = f
end

function ofelia.sheight(f)
  sheight = f
end

function ofelia.pos(p)
  x, y, z = p.x, p.y, p.z
end

function ofelia.image(p)
  image = p
end