local log = ofLog("ofLostFocus")
local canvas = ofCanvas(this)
local order, enabled = 50, true

function M.new()
  local args = canvas:getArgs()
  if #args == 1 then
    order = args[1]
  elseif #args == 2 then
    order, enabled = args[1], args[2] ~= 0
  elseif #args ~= 0 then
    log:error("requires 1 or 2 creation arguments")
  end
  ofWindow.addListener("lostFocus", this, order)
end

function M.setOrder(f)
  order = f
  ofWindow.addListener("lostFocus", this, order)
end

function M.setEnabled(b)
  enabled = b ~= 0
end

function M.free()
  ofWindow.removeListener("lostFocus", this)
end

function M.lostFocus()
  if not enabled then
    return
  end
  return nil
end