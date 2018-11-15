local objName = "of.beginSaveScreenAsPDF"
local log = pd.Log()
local canvas = pd.Canvas(this)
local filename, bMultipage, b3D, outputsize = "", false, false, of.Rectangle(0, 0, 0, 0)

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    filename = args[1]
  elseif #args == 2 then
    filename, bMultipage = args[1], args[2] ~= 0
  elseif #args == 3 then
    filename, bMultipage, b3D = args[1], args[2] ~= 0, args[3] ~= 0
  elseif #args == 5 then
    filename, bMultipage, b3D = args[1], args[2] ~= 0, args[3] ~= 0
    outputsize:set(0, 0, args[4], args[5])
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1, 2, 3 or 5 creation arguments")
  end
end

function ofelia.bang()
  of.beginSaveScreenAsPDF(filename, bMultipage, b3D, outputsize)
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

function ofelia.outputsize(p)
  outputsize:set(p.x, p.y, p.width, p.height)
end