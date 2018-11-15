local canvas = pd.Canvas(this, 1)
local currentDir = canvas:getDir() .. "/"
ofelia.image = nil

function ofelia.new()
  pd.OFWindow.addListener("exit", this)
end

function ofelia.clear()
  if type(ofelia.image) == "userdata" then
    ofelia.image:clear()
  end
  ofelia.image = nil
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

function ofelia.load(s)
  if type(ofelia.image) ~= "userdata" then
    ofelia.image = of.Image()
  end
  if isPathRelative(s) then
    s = currentDir .. s
  end
  return ofelia.image:load(s)
end