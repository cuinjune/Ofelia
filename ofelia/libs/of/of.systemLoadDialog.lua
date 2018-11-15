local objName = "of.systemLoadDialog"
local log = pd.Log()
local canvas = pd.Canvas(this)
local windowTitle, bFolderSelection, defaultPath = "", false, ""

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    windowTitle = args[1]
  elseif #args == 2 then
    windowTitle, bFolderSelection = args[1], args[2] ~= 0
  elseif #args == 3 then
    windowTitle, bFolderSelection, defaultPath = args[1], args[2] ~= 0, args[3]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 2 or 3 creation arguments")
  end
end

function ofelia.bang()
  return of.systemLoadDialog(windowTitle, bFolderSelection, defaultPath)
end

function ofelia.windowTitle(s)
  windowTitle = s
end

function ofelia.bFolderSelection(b)
  bFolderSelection = b ~= 0
end

function ofelia.defaultPath(s)
  defaultPath = s
end