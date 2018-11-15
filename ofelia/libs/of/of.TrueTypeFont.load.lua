local objName = "of.TrueTypeFont.load"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local fileName, fontSize, bAntiAliased, bFullCharacterSet, makeContours, simplifyAmt, dpi = "", 0, true, true, false, 0.3, 0

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 2 then
    fileName, fontSize = args[1], args[2]
  elseif #args == 3 then
    fileName, fontSize, bAntiAliased = args[1], args[2], args[3] ~= 0
  elseif #args == 4 then
    fileName, fontSize, bAntiAliased, bFullCharacterSet = args[1], args[2], args[3] ~= 0, args[4] ~= 0
  elseif #args == 5 then
    fileName, fontSize, bAntiAliased, bFullCharacterSet, makeContours = args[1], args[2], args[3] ~= 0, args[4] ~= 0, args[5] ~= 0
  elseif #args == 6 then
    fileName, fontSize, bAntiAliased, bFullCharacterSet, makeContours, simplifyAmt = args[1], args[2], args[3] ~= 0, args[4] ~= 0, args[5] ~= 0, args[6]
  elseif #args == 7 then
    fileName, fontSize, bAntiAliased, bFullCharacterSet, makeContours, simplifyAmt, dpi = args[1], args[2], args[3] ~= 0, args[4] ~= 0, args[5] ~= 0, args[6], args[7]
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2, 3, 4, 5, 6 or 7 creation arguments")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return false
  end
  return font:load(fileName, fontSize, bAntiAliased, bFullCharacterSet, makeContours, simplifyAmt, dpi)
end

function ofelia.fileName(s)
  fileName = s
end

function ofelia.fontSize(i)
  fontSize = i
end

function ofelia.bAntiAliased(b)
  bAntiAliased = b ~= 0
end

function ofelia.bFullCharacterSet(b)
  bFullCharacterSet = b ~= 0
end

function ofelia.makeContours(b)
  makeContours = b ~= 0
end

function ofelia.simplifyAmt(f)
  simplifyAmt = f
end

function ofelia.dpi(i)
  dpi = i
end

function ofelia.settings(p)
  fileName, fontSize, bAntiAliased, bFullCharacterSet, makeContours, simplifyAmt, dpi = p.fontName, p.fontSize, p.antialiased, true, p.contours, p.simplifyAmt, p.dpi
end

function ofelia.font(p)
  font = p
end