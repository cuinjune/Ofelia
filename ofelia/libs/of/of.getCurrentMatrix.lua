local objName = "of.getCurrentMatrix"
local log = pd.Log()
local canvas = pd.Canvas(this)
local matrixMode = 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    matrixMode = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  return of.getCurrentMatrix(matrixMode)
end

function ofelia.matrixMode(i)
  matrixMode = i
end