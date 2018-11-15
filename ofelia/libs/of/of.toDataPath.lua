local objName = "of.toDataPath"
local log = pd.Log()
local canvas = pd.Canvas(this)
local path, makeAbsolute = "", false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    path = args[1]
  elseif #args == 2 then
    path, makeAbsolute = args[1], args[2] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 1 or 2 creation arguments")
  end
end

function ofelia.bang()
  of.toDataPath(path, makeAbsolute)
  return nil
end

function ofelia.path(s)
  path = s
end

function ofelia.makeAbsolute(b)
  makeAbsolute = b ~= 0
end