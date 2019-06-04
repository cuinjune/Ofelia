local objName = "ofNoise"
local log = pdLog()

function ofelia.float(f)
  return ofNoise(f)
end

function ofelia.list(fv)
  if #fv == 2 then
    return ofNoise(fv[1], fv[2])
  elseif #fv == 3 then
    return ofNoise(fv[1], fv[2], fv[3])
  elseif #fv == 4 then
    return ofNoise(fv[1], fv[2], fv[3], fv[4])
  else
    log:error(objName .. " : requires 2, 3 or 4 arguments")
  end
end