local objName = "of.TrueTypeFont"
local font = nil

function ofelia.new()
  pd.OFWindow.addListener("exit", this)
end

function ofelia.clear()
  font = nil
end

function ofelia.free()
  ofelia.clear()
  pd.OFWindow.removeListener("exit", this)
end

function ofelia.exit()
  ofelia.clear()
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    font = of.TrueTypeFont()
  end
  return font
end