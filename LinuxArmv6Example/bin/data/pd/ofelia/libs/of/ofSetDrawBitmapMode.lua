local log = pdLog("ofSetDrawBitmapMode")
local canvas = pdCanvas(this)
local mode = OF_BITMAPMODE_SCREEN

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    ofelia.mode(args[1])
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  ofSetDrawBitmapMode(mode)
  return nil
end

function ofelia.mode(a)
  if type(a) == "number" then
    mode = a
  elseif a == "OF_BITMAPMODE_SIMPLE" then
    mode = OF_BITMAPMODE_SIMPLE
  elseif a == "OF_BITMAPMODE_SCREEN" then
    mode = OF_BITMAPMODE_SCREEN
  elseif a == "OF_BITMAPMODE_VIEWPORT" then
    mode = OF_BITMAPMODE_VIEWPORT
  elseif a == "OF_BITMAPMODE_MODEL" then
    mode = OF_BITMAPMODE_MODEL
  elseif a == "OF_BITMAPMODE_MODEL_BILLBOARD" then
    mode = OF_BITMAPMODE_MODEL_BILLBOARD
  else
    log:error("unknown mode : " .. a)
  end
end