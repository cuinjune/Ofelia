local objName = "of.setCylinderResolution"
local log = pd.Log()
local canvas = pd.Canvas(this)
local radiusSegments, heightSegments, capSegments = 8, 4, 2

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    radiusSegments, heightSegments = args[1], args[2]
  elseif #args == 3 then
    radiusSegments, heightSegments, capSegments = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 or 3 creation arguments")
  end
end

function ofelia.bang()
  of.setCylinderResolution(radiusSegments, heightSegments, capSegments)
  return nil
end

function ofelia.radiusSegments(i)
  radiusSegments = i
end

function ofelia.heightSegments(i)
  heightSegments = i
end

function ofelia.capSegments(i)
  capSegments = i
end