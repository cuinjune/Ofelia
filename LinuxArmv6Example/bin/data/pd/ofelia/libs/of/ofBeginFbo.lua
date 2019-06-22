local log = pdLog("ofBeginFbo")
local canvas = pdCanvas(this)
local name, mode = "", OF_FBOMODE_PERSPECTIVE | OF_FBOMODE_MATRIXFLIP

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    name = args[1]
  elseif #args == 2 then
    name = args[1]
    ofelia.mode(args[2])
  elseif #args ~= 0 then
    log:error("requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  local m = require(name)
  m.fbo:beginFbo(mode)
  return nil
end

function ofelia.name(s)
  name = s
end

function ofelia.mode(a)
  if type(a) == "number" then
    mode = a
  elseif a == "OF_FBOMODE_NODEFAULTS" then
    mode = OF_FBOMODE_NODEFAULTS
  elseif a == "OF_FBOMODE_PERSPECTIVE" then
    mode = OF_FBOMODE_PERSPECTIVE
  elseif a == "OF_FBOMODE_MATRIXFLIP" then
    mode = OF_FBOMODE_MATRIXFLIP
  else
    log:error("unknown mode : " .. a)
  end
end