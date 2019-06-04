local objName = "ofSetDataPathRoot"
local log = pdLog()
local canvas = pdCanvas(this)
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
  ofSetDataPathRoot(newRoot)
  return nil
end

function ofelia.newRoot(s)
  newRoot = s
end