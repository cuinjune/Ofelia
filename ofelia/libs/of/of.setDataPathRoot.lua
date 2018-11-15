local objName = "of.setDataPathRoot"
local log = pd.Log()
local canvas = pd.Canvas(this)
local newRoot = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    newRoot = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  of.setDataPathRoot(newRoot)
  return nil
end

function ofelia.newRoot(s)
  newRoot = s
end