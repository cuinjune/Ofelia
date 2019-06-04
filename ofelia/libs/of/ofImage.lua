local objName = "ofImage"
local log = pdLog()
local canvas = pdCanvas(this, 1)
local currentDir = canvas:getDir() .. "/"
local outlet = pdOutlet(this)
M.image = ofImage()

local function isPathRelative(path)
  if path:match"^[~/\\]" or path:match"^%a:[/\\]" then
    return false
  end
    return true
end

function ofelia.new()
  ofWindow.addListener("exit", this)
end

function ofelia.bang()
  return M.image
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
  M.image:clear()
  M.image = nil
  ofWindow.removeListener("exit", this)
end

function ofelia.exit()
  M.image:clear()
end

function ofelia.allocate(av)
  local width, height, format = av[1], av[2], av[3]
  if type(format) == "number" then
    format = format
  elseif format == "OF_IMAGE_GRAYSCALE" then
    format = OF_IMAGE_GRAYSCALE
  elseif format == "OF_IMAGE_COLOR" then
    format = OF_IMAGE_COLOR
  elseif format == "OF_IMAGE_COLOR_ALPHA" then
    format = OF_IMAGE_COLOR_ALPHA
  elseif format == "OF_IMAGE_UNDEFINED" then
    format = OF_IMAGE_UNDEFINED
  else
    log:error(objName .. " : unknown format : " .. format)
    return
  end
  M.image:allocate(width, height, format) 
end

function ofelia._clear()
  M.image:clear()
end

function ofelia.load(av)
  local fileName, buffer, settings = nil, nil, ofImageLoadSettings()
  if type(av) == "table" then
    if type(av[1]) == "userdata" then
      buffer = av[1]
    else
      fileName = av[1]
    end
    settings = av[2]
  elseif type(av) == "userdata" then
    buffer = av
  else
    fileName = av
  end
  if type(fileName) ~= "nil" then
    if isPathRelative(fileName) then
      fileName = currentDir .. fileName
    end
    outlet:outletAnything(0, {"load", M.image:load(fileName, settings)})
  else
    outlet:outletAnything(0, {"load", M.image:load(buffer, settings)})
  end
end

function ofelia.update()
  M.image:update()
end

function ofelia.setUseTexture(b)
  M.image:setUseTexture(b ~= 0)
end

function ofelia.setCompression(a)
  local format = a
  if type(format) == "number" then
    format = format
  elseif format == "OF_COMPRESS_NONE" then
    format = OF_COMPRESS_NONE
  elseif format == "OF_COMPRESS_SRGB" then
    format = OF_COMPRESS_SRGB
  elseif format == "OF_COMPRESS_ARB" then
    format = OF_COMPRESS_ARB
  else
    log:error(objName .. " : unknown format : " .. format)
    return
  end
  M.image:setCompression(format)
end

function ofelia.setColor(iv)
  if #iv == 6 then
    M.image:setColor(iv[1], iv[2], ofColor(iv[3], iv[4], iv[5], iv[6]))
  elseif #iv == 5 then
    M.image:setColor(iv[1], ofColor(iv[2], iv[3], iv[4], iv[5]))
  elseif #iv == 4 then
    M.image:setColor(ofColor(iv[1], iv[2], iv[3], iv[4]))
  else
    log:error(objName .. " : 'setColor' requires 4, 5 or 6 arguments")
  end
end

function ofelia.setFromPixels(p)
  M.image:setFromPixels(p)
end

function ofelia.grabScreen(iv)
  local x, y, width, height = iv[1], iv[2], iv[3], iv[4]
  M.image:grabScreen(x, y, width, height)
end

function ofelia.setImageType(a)
  local format = a
  if type(format) == "number" then
    format = format
  elseif format == "OF_IMAGE_GRAYSCALE" then
    format = OF_IMAGE_GRAYSCALE
  elseif format == "OF_IMAGE_COLOR" then
    format = OF_IMAGE_COLOR
  elseif format == "OF_IMAGE_COLOR_ALPHA" then
    format = OF_IMAGE_COLOR_ALPHA
  elseif format == "OF_IMAGE_UNDEFINED" then
    format = OF_IMAGE_UNDEFINED
  else
    log:error(objName .. " : unknown format : " .. format)
    return
  end
  M.image:setImageType(format) 
end

function ofelia.resize(iv)
  M.image:resize(iv[1], iv[2])
end

function ofelia.crop(iv)
  M.image:crop(iv[1], iv[2], iv[3], iv[4])
end

function ofelia.cropFrom(av)
  M.image:cropFrom(av[1], av[2], av[3], av[4], av[5])
end

function ofelia.rotate90(i)
  M.image:rotate90(i)
end

function ofelia.mirror(iv)
  M.image:mirror(iv[1] ~= 0, iv[2] ~= 0)
end

function ofelia.setAnchorPercent(fv)
  M.image:setAnchorPercent(fv[1], fv[2])
end

function ofelia.setAnchorPoint(fv)
  M.image:setAnchorPoint(fv[1], fv[2])
end

function ofelia.resetAnchor()
  M.image:resetAnchor()
end

function ofelia.save(s)
  if isPathRelative(s) then
    s = currentDir .. s
  end
  outlet:outletAnything(0, {"save", M.image:save(s)})
end