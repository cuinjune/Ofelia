local objName = "of.Image.setAnchorPercent"
local log = pd.Log()
local canvas = pd.Canvas(this)
local image = nil
local xPct, yPct = 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    xPct, yPct = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  if type(image) ~= "userdata" then
    return nil
  end
  image:setAnchorPercent(xPct, yPct)
  return nil
end

function ofelia.xPct(f)
  xPct = f
end

function ofelia.yPct(f)
  yPct = f
end

function ofelia.image(p)
  image = p
end