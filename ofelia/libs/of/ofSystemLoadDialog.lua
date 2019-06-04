local objName = "ofSystemLoadDialog"
local log = pdLog()
local canvas = pdCanvas(this)
local bFolderSelection, defaultPath = false, ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    bFolderSelection = args[1] ~= 0
  elseif #args == 2 then
    bFolderSelection, defaultPath = args[1] ~= 0, args[2]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
end

function ofelia.symbol(s)
  return ofSystemLoadDialog(s, bFolderSelection, defaultPath)
end

function ofelia.bFolderSelection(b)
  bFolderSelection = b ~= 0
end

function ofelia.defaultPath(s)
  defaultPath = s
end