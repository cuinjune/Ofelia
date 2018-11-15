local objName = "of.scale"
local log = pd.Log()
local canvas = pd.Canvas(this)
local xAmnt, yAmnt, zAmnt = 0, 0, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    xAmnt, yAmnt, zAmnt = args[1], args[1], args[1]
  elseif #args == 2 then
    xAmnt, yAmnt = args[1], args[2]
  elseif #args == 3 then
    xAmnt, yAmnt, zAmnt = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 2 or 3 creation arguments")
  end
end

function ofelia.bang()
  of.scale(xAmnt, yAmnt, zAmnt)
  return nil
end

function ofelia.xAmnt(f)
  xAmnt = f
end

function ofelia.yAmnt(f)
  yAmnt = f
end

function ofelia.zAmnt(f)
  zAmnt = f
end

function ofelia.amount(f)
  xAmnt, yAmnt, zAmnt = f, f, f
end

function ofelia.pos(p)
  xAmnt, yAmnt, zAmnt = p.x, p.y, p.z
end