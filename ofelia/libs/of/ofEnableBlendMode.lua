local objName = "ofEnableBlendMode"
local log = pdLog()
local canvas = pdCanvas(this)
local mode = OF_BLENDMODE_ALPHA

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    ofelia.mode(args[1])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  ofEnableBlendMode(mode)
  return nil
end

function ofelia.mode(a)
  if type(a) == "number" then
    mode = a
  elseif a == "OF_BLENDMODE_DISABLED" then
    mode = OF_BLENDMODE_DISABLED
  elseif a == "OF_BLENDMODE_ALPHA" then
    mode = OF_BLENDMODE_ALPHA
  elseif a == "OF_BLENDMODE_ADD" then
    mode = OF_BLENDMODE_ADD
  elseif a == "OF_BLENDMODE_SUBTRACT" then
    mode = OF_BLENDMODE_SUBTRACT
  elseif a == "OF_BLENDMODE_MULTIPLY" then
    mode = OF_BLENDMODE_MULTIPLY
  elseif a == "OF_BLENDMODE_SCREEN" then
    mode = OF_BLENDMODE_SCREEN
  else
    log:error(objName .. " : unknown mode : " .. a)
  end
end