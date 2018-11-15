local objName = "of.setSphereResolution"
local log = pd.Log()
local canvas = pd.Canvas(this)
local res = 20

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    res = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setSphereResolution(res)
  return nil
end

function ofelia.res(i)
  res = i
end