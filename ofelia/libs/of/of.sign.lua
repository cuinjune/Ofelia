local objName = "of.sign"
local log = pd.Log()
local canvas = pd.Canvas(this)
local n = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    n = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.sign(n)
end

function ofelia.n(f)
  n = f
end