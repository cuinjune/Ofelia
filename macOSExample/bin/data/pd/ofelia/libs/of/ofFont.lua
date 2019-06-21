local log = pdLog("ofFont")
local canvas = pdCanvas(this, 1)
local currentDir = canvas:getDir() .. "/"
local outlet = pdOutlet(this)
M.font = ofTrueTypeFont()

local function isPathRelative(path)
  if path:match"^[~/\\]" or path:match"^%a:[/\\]" then
    return false
  end
    return true
end

function ofelia.bang()
  return M.font
end

function ofelia.float(f)
  return {f, ofelia.bang()}
end

function ofelia.symbol(s)
  return {s, ofelia.bang()}
end

function ofelia.pointer(p)
  return {p, ofelia.bang()}
end

function ofelia.list(av)
  av[#av+1] = ofelia.bang()
  return av
end

function ofelia.free()
  M.font = nil
end

function ofelia.load(av)
  if type(av) == "table" then
    local fileName = av[1]
    if isPathRelative(fileName) then
      fileName = currentDir .. fileName
    end
    if #av == 2 then
      outlet:outletAnything(0, {"load", M.font:load(fileName, av[2])})
    elseif #av == 7 then
      outlet:outletAnything(0, {"load", M.font:load(fileName, av[2], av[3] ~= 0, av[4] ~= 0, av[5] ~= 0, av[6], av[7])})
    else
      log:error("'load' requires 2 or 7 arguments")
    end
  else
    outlet:outletAnything(0, {"load", M.font:load(av)})
  end
end

function ofelia.setGlobalDpi(i)
  ofTrueTypeFont.setGlobalDpi(i)
end

function ofelia.setLineHeight(f)
  M.font:setLineHeight(f)
end

function ofelia.setLetterSpacing(f)
  M.font:setLetterSpacing(f)
end

function ofelia.setSpaceSize(f)
  M.font:setSpaceSize(f)
end

function ofelia.setDirection(i)
  M.font:setDirection(i)
end