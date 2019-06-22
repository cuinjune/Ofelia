local log = pdLog("ofSetMatrixMode")
local canvas = pdCanvas(this)
local mode = OF_MATRIX_MODELVIEW

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    ofelia.mode(args[1])
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetMatrixMode(mode)
  return nil
end

function ofelia.mode(a)
  if type(a) == "number" then
    mode = a
  elseif a == "OF_MATRIX_MODELVIEW" then
    mode = OF_MATRIX_MODELVIEW
  elseif a == "OF_MATRIX_PROJECTION" then
    mode = OF_MATRIX_PROJECTION
  elseif a == "OF_MATRIX_TEXTURE" then
    mode = OF_MATRIX_TEXTURE
  else
    log:error("unknown mode : " .. a)
  end
end