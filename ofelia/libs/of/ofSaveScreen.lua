local objName = "ofSaveScreen"
local log = pdLog()
local canvas = pdCanvas(this)
local canvas2 = pdCanvas(this, 1)
local currentDir = canvas2:getDir() .. "/"
local filename = ""

local function isPathRelative(path)
  if path:match"^[~/\\]" or path:match"^%a:[/\\]" then
    return false
  end
    return true
end

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    filename = args[1]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 creation argument")
  end
end

function ofelia.bang()
  if isPathRelative(filename) then
    filename = currentDir .. filename
  end
  ofSaveScreen(filename)
  return nil
end

function ofelia.filename(s)
  filename = s
end