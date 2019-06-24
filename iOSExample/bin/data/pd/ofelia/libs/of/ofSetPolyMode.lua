local log = pdLog("ofSetPolyMode")
local canvas = pdCanvas(this)
local mode = OF_POLY_WINDING_ODD

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    ofelia.mode(args[1])
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetPolyMode(mode)
  return nil
end

function ofelia.mode(a)
  if type(a) == "number" then
    mode = a
  elseif a == "OF_POLY_WINDING_ODD" then
    mode = OF_POLY_WINDING_ODD
  elseif a == "OF_POLY_WINDING_NONZERO" then
    mode = OF_POLY_WINDING_NONZERO
  elseif a == "OF_POLY_WINDING_POSITIVE" then
    mode = OF_POLY_WINDING_POSITIVE
  elseif a == "OF_POLY_WINDING_NEGATIVE" then
    mode = OF_POLY_WINDING_NEGATIVE
  elseif a == "OF_POLY_WINDING_ABS_GEQ_TWO" then
    mode = OF_POLY_WINDING_ABS_GEQ_TWO
  else
    log:error("unknown mode : " .. a)
  end
end