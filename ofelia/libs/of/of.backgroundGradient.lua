local objName = "of.backgroundGradient"
local log = pd.Log()
local canvas = pd.Canvas(this)
local c1, c2, mode = of.Color(255), of.Color(0), 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 7 then
    c1:set(args[1], args[2], args[3])
    c2:set(args[4], args[5], args[6])
    mode = args[7]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 7 creation arguments")
  end
end

function ofelia.bang()
  of.backgroundGradient(c1, c2, mode)
  return nil
end

function ofelia.c1(p)
  c1:set(p.r, p.g, p.b, p.a)
end

function ofelia.c2(p)
  c2:set(p.r, p.g, p.b, p.a)
end

function ofelia.mode(i)
  mode = i
end