local objName = "of.endShape"
local log = pd.Log()
local canvas = pd.Canvas(this)
local bClose = false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    bClose = args[1] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.endShape(bClose)
  return nil
end

function ofelia.bClose(b)
  bClose = b ~= 0
end