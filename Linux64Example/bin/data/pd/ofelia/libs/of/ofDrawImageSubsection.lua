local log = pdLog("ofDrawImageSubsection")
local canvas = pdCanvas(this)
local name, pos, width, height, sx, sy, swidth, sheight = "", ofVec3f(), 0, 0, 0, 0, nil, nil

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args == 7 then
    name, width, height, sx, sy = args[1], args[4], args[5], args[6], args[7]
    pos:set(args[2], args[3])
  elseif #args == 8 then
    name, width, height, sx, sy = args[1], args[5], args[6], args[7], args[8]
    pos:set(args[2], args[3], args[4])
  elseif #args == 9 then
    name, width, height, sx, sy, swidth, sheight = args[1], args[4], args[5], args[6], args[7], args[8], args[9]
    pos:set(args[2], args[3])
  elseif #args == 10 then
    name, width, height, sx, sy, swidth, sheight = args[1], args[5], args[6], args[7], args[8], args[9], args[10]
    pos:set(args[2], args[3], args[4])
  elseif #args ~= 0 then
    log:error("requires 1, 7, 8, 9 or 10 creation arguments")
  end
end

function ofelia.bang()
  local m = require(name)
  if type(swidth) == "nil" or type(sheight) == "nil" then
    m.image:drawSubsection(pos.x, pos.y, pos.z, width, height, sx, sy)
    return nil
  end
  m.image:drawSubsection(pos.x, pos.y, pos.z, width, height, sx, sy, swidth, sheight)
  return nil
end

function ofelia.name(s)
  name = s
end

function ofelia.x(f)
  pos.x = f
end

function ofelia.y(f)
  pos.y = f
end

function ofelia.z(f)
  pos.z = f
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

function ofelia.pos(fv)
  if #fv == 2 then
    pos:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    pos:set(fv[1], fv[2], fv[3])
  else
    log:error("'pos' requires 2 or 3 arguments")
  end
end