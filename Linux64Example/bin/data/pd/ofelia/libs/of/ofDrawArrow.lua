local log = pdLog("ofDrawArrow")
local canvas = pdCanvas(this)
local from, to, headSize = ofVec3f(), ofVec3f(), 0.05

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 4 then
    from:set(args[1], args[2])
    to:set(args[3], args[4])
  elseif #args == 5 then
    from:set(args[1], args[2])
    to:set(args[3], args[4])
    headSize = args[5]
  elseif #args == 6 then
    from:set(args[1], args[2], args[3])
    to:set(args[4], args[5], args[6])
  elseif #args == 7 then
    from:set(args[1], args[2], args[3])
    to:set(args[4], args[5], args[6])
    headSize = args[7]
  elseif #args ~= 0 then
    log:error("requires 4, 5, 6 or 7 creation arguments")
  end
end

function ofelia.bang()
  ofDrawArrow(from:vec3(), to:vec3(), headSize)
  return nil
end

function ofelia.from(fv)
  if #fv == 2 then
    from:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    from:set(fv[1], fv[2], fv[3])
  else
    log:error("'from' requires 2 or 3 arguments")
  end
end

function ofelia.to(fv)
  if #fv == 2 then
    to:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    to:set(fv[1], fv[2], fv[3])
  else
    log:error("'to' requires 2 or 3 arguments")
  end
end

function ofelia.headSize(f)
  headSize = f
end