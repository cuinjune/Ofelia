local objName = "ofSetPlaneResolution"
local log = pdLog()
local canvas = pdCanvas(this)
local columns, rows = 6, 4

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    columns, rows = args[1], args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  ofSetPlaneResolution(columns, rows)
  return nil
end

function ofelia.columns(i)
  columns = i
end

function ofelia.rows(i)
  rows = i
end