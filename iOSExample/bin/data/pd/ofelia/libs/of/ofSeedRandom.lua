local log = pdLog("ofSeedRandom")
local canvas = pdCanvas(this)
local val = nil

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    val = args[1]
  elseif #args ~= 0 then
    log:error("requires 1 creation argument")
  end
end

function ofelia.bang()
  if val == nil then
    ofSeedRandom()
  else
    ofSeedRandom(val)
  end
  return nil
end

function ofelia.val(i)
  val = i
end