local objName = "of.TrueTypeFont.getCharacterAsPoints"
local log = pd.Log()
local canvas = pd.Canvas(this)
local font = nil
local character, vFlip, filled = 0, true, true

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    character = args[1]
  elseif #args == 3 then
    character, vFlip, filled = args[1], args[2] ~= 0, args[3] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 3 creation arguments")
  end
end

function ofelia.bang()
  if type(font) ~= "userdata" then
    return of.Path()
  end
  return font:getCharacterAsPoints(character, vFlip, filled)
end

function ofelia.character(i)
  character = i
end

function ofelia.vFlip(b)
  vFlip = b ~= 0
end

function ofelia.filled(b)
  filled = b ~= 0
end

function ofelia.font(p)
  font = p
end