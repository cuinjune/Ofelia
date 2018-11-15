local objName = "of.trimBack"
local log = pd.Log()
local canvas = pd.Canvas(this)
local src, locale = "", ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    src, locale = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  return of.trimBack(src, locale)
end

function ofelia.src(s)
  src = s
end

function ofelia.locale(s)
  locale = s
end