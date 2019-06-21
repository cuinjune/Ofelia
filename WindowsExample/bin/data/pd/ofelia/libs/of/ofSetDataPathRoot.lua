local log = pdLog("ofSetDataPathRoot")
local canvas = pdCanvas(this)
local newRoot = ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    newRoot = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetDataPathRoot(newRoot)
  return nil
end

function ofelia.newRoot(s)
  newRoot = s
end