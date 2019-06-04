local objName = "ofBeginSaveScreenAsPDF"
local log = pdLog()
local canvas = pdCanvas(this)
local canvas2 = pdCanvas(this, 1)
local currentDir = canvas2:getDir() .. "/"
local filename, bMultipage, b3D, outputsize = "", false, false, ofRectangle(0, 0, 0, 0)

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
  elseif #args == 2 then
    filename, bMultipage = args[1], args[2] ~= 0
  elseif #args == 3 then
    filename, bMultipage, b3D = args[1], args[2] ~= 0, args[3] ~= 0
  elseif #args == 7 then
    filename, bMultipage, b3D = args[1], args[2] ~= 0, args[3] ~= 0
    outputsize:set(args[4], args[5], args[6], args[7])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 2, 3 or 7 creation arguments")
  end
end

function ofelia.bang()
  if isPathRelative(filename) then
    filename = currentDir .. filename
  end
  ofBeginSaveScreenAsPDF(filename, bMultipage, b3D, outputsize)
  return nil
end

function ofelia.filename(s)
  filename = s
end

function ofelia.bMultipage(b)
  bMultipage = b ~= 0
end

function ofelia.b3D(b)
  b3D = b ~= 0
end

function ofelia.outputsize(fv)
  outputsize:set(fv[1], fv[2], fv[3], fv[4])
end