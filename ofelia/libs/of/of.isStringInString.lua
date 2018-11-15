local objName = "of.isStringInString"
local log = pd.Log()
local canvas = pd.Canvas(this)
local haystack, needle = "", ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    haystack, needle = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  return of.isStringInString(haystack, needle)
end

function ofelia.haystack(s)
  haystack = s
end

function ofelia.needle(s)
  needle = s
end