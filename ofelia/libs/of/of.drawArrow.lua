local objName = "of.drawArrow"
local log = pd.Log()
local canvas = pd.Canvas(this)
local from, to, headSize = of.Vec3f(0, 0, 0), of.Vec3f(0, 0, 0), 0.05

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 6 then
    from:set(args[1], args[2], args[3])
    to:set(args[4], args[5], args[6])
  elseif #args == 7 then
    from:set(args[1], args[2], args[3])
    to:set(args[4], args[5], args[6])
    headSize = args[7]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 6 or 7 creation arguments")
  end
end

function ofelia.bang()
  of.drawArrow(from:vec3(), to:vec3(), headSize)
  return nil
end

function ofelia.from(p)
  from:set(p.x, p.y, p.z)
end

function ofelia.to(p)
  to:set(p.x, p.y, p.z)
end

function ofelia.headSize(f)
  headSize = f
end