local objName = "ofDrawBitmapString"
local log = pdLog()
local canvas = pdCanvas(this)
local text, pos = "", ofVec3f()

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    text = args[1]
    pos:set(args[2], args[3])
  elseif #args == 4 then
    text = args[1]
    pos:set(args[2], args[3], args[4])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 3 or 4 creation arguments")
  end
end

function ofelia.bang()
  ofDrawBitmapString(text, pos:vec3())
  return nil
end

function ofelia.text(s)
  text = s
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

function ofelia.pos(fv)
  if #fv == 2 then
    pos:set(fv[1], fv[2], 0)
  elseif #fv == 3 then
    pos:set(fv[1], fv[2], fv[3])
  else
    log:error(objName .. " : 'pos' requires 2 or 3 arguments")
  end
end