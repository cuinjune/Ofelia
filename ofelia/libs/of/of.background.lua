local objName = "of.background"
local log = pd.Log()
local canvas = pd.Canvas(this)
local r, g, b, a = 200, 200, 200, 255

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    r, g, b = args[1], args[1], args[1]
  elseif #args == 2 then
    r, g, b, a = args[1], args[1], args[1], args[2]
  elseif #args == 3 then
    r, g, b = args[1], args[2], args[3]
  elseif #args == 4 then
    r, g, b, a = args[1], args[2], args[3], args[4]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 2, 3 or 4 creation arguments")
  end
end

function ofelia.bang()
  of.background(r, g, b, a)
  return nil
end

function ofelia.r(i)
  r = i
end

function ofelia.g(i)
  g = i
end

function ofelia.b(i)
  b = i
end

function ofelia.a(i)
  a = i
end

function ofelia.brightness(i)
  r, g, b = i, i, i
end

function ofelia.alpha(i)
  a = i
end

function ofelia.color(p)
  r, g, b, a = p.r, p.g, p.b, p.a
end