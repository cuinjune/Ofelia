local log = pdLog("ofScale")
local canvas = pdCanvas(this)
local pos = ofVec3f(1, 1, 1)

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    pos:set(args[1])
  elseif #args == 2 then
    pos:set(args[1], args[2])
  elseif #args == 3 then
    pos:set(args[1], args[2], args[3])
  elseif #args ~= 0 then
    log:error("requires 1, 2 or 3 creation arguments")
  end
end

function ofelia.bang()
  ofScale(pos:vec3())
  return nil
end

function ofelia.xAmnt(f)
  pos.x = f
end

function ofelia.yAmnt(f)
  pos.y = f
end

function ofelia.zAmnt(f)
  pos.z = f
end

function ofelia.amount(f)
  pos:set(f)
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