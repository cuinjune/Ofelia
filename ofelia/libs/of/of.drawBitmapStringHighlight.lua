local objName = "of.drawBitmapStringHighlight"
local log = pd.Log()
local canvas = pd.Canvas(this)
local text, x, y, background, foreground = "", 0, 0, of.Color(255), of.Color(0)

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 3 then
    text, x, y = args[1], args[2], args[3]
  elseif #args == 9 then
    text, x, y = args[1], args[2], args[3]
    background:set(args[4], args[5], args[6])
    foreground:set(args[7], args[8], args[9])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 3 or 9 creation arguments")
  end
end

function ofelia.bang()
  of.drawBitmapStringHighlight(text, x, y, background, foreground)
  return nil
end

function ofelia.text(s)
  text = s
end

function ofelia.x(i)
  x = i
end

function ofelia.y(i)
  y = i
end

function ofelia.background(p)
  background:set(p.r, p.g, p.b, p.a)
end

function ofelia.foreground(p)
  foreground:set(p.r, p.g, p.b, p.a)
end