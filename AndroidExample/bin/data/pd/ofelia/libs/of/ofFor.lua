local log = pdLog("ofFor")
local canvas = pdCanvas(this)
local outlet = pdOutlet(this)
local init, target, amount, stop = 0, pdGetMaxFloat(), 1, false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    init, target = args[1], args[2]
  elseif #args == 3 then
    init, target, amount = args[1], args[2], args[3]
  elseif #args ~= 0 then
    log:error("requires 2 or 3 creation arguments")
  end
end

function ofelia.bang()
  for f = init, target, amount do
    if stop == true then
      stop = false
      return
    end
    outlet:outletFloat(0, f)
  end 
end

function ofelia.list(fv)
  if #fv == 2 then
    init, target, amount = fv[1], fv[2], 1
  elseif #fv == 3 then
    init, target, amount = fv[1], fv[2], fv[3]
  end
  ofelia.bang()
end

function ofelia.stop()
  stop = true
end