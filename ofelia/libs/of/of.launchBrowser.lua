local objName = "of.launchBrowser"
local log = pd.Log()
local canvas = pd.Canvas(this)
local url, uriEncodeQuery = "", false

function ofelia.new()
  local args = canvas:getArgs()
  if #args == 1 then
    url = args[1]
  elseif #args == 2 then
    url, uriEncodeQuery = args[1], args[2] ~= 0
  elseif #args ~= 0 then
    log:error(objName .. " : requires 2 creation arguments")
  end
end

function ofelia.bang()
  of.launchBrowser(url, uriEncodeQuery)
end

function ofelia.url(s)
  url = s
end

function ofelia.uriEncodeQuery(b)
  uriEncodeQuery = b ~= 0
end