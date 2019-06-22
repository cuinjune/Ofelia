local log = pdLog("ofDrawBox")
local canvas = pdCanvas(this)
local pos, width, height, depth = ofVec3f(), 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    width, height, depth = args[1], args[1], args[1]
  elseif #args == 3 then
    width, height, depth = args[1], args[2], args[3]
  elseif #args == 4 then
    pos:set(args[1], args[2], args[3])
    width, height, depth = args[4], args[4], args[4]
  elseif #args == 6 then
    pos:set(args[1], args[2], args[3])
    width, height, depth = args[4], args[5], args[6]
  elseif #args ~= 0 then
    log:error("requires 1, 3, 4 or 6 creation arguments")
  end
end

function ofelia.bang()
  ofDrawBox(pos:vec3(), width, height, depth)
  return nil
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

function ofelia.depth(f)
  depth = f
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