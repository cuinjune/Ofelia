local canvas = pd.Canvas(this, 1)
local currentDir = canvas:getDir() .. "/"
ofelia.font = nil

function ofelia.new()
  pd.OFWindow.addListener("exit", this)
end

function ofelia.clear()
  ofelia.font = nil
end

function ofelia.free()
  ofelia.clear()
  pd.OFWindow.removeListener("exit", this)
end

function ofelia.exit()
  ofelia.clear()
end

local function isPathRelative(path)
  if path:match"^[~/\\]" or path:match"^%a:[/\\]" then
    return false
  end
    return true
end

function ofelia.load(av)
  if type(ofelia.font) ~= "userdata" then
    ofelia.font = of.TrueTypeFont()
  end
  if isPathRelative(av[1]) then
    av[1] = currentDir .. av[1]
  end
  return ofelia.font:load(av[1], av[2], true, true, true, 0.3, 0)
end