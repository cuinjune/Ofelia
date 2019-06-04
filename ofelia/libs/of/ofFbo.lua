local objName = "ofFbo"
local log = pdLog()
local canvas = pdCanvas(this, 1)
local currentDir = canvas:getDir() .. "/"
local outlet = pdOutlet(this)
M.fbo = ofFbo()

function ofelia.new()
  ofWindow.addListener("exit", this)
end

function ofelia.bang()
  return M.fbo
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
  M.fbo:clear()
  M.fbo = nil
  ofWindow.removeListener("exit", this)
end

function ofelia.exit()
  M.fbo:clear()
end

function ofelia.allocate(av)
  local width, height, format, numSamples = 0, 0, GL_RGBA, 0
  if #av == 2 then
    width, height = av[1], av[2]
  elseif #av == 3 then
    width, height, format = av[1], av[2], av[3]
  elseif #av == 4 then
    width, height, format, numSamples = av[1], av[2], av[3], av[4]
  else
    log:error(objName .. " : 'allocate' requires 2, 3 or 4 arguments")
    return
  end
  if type(format) == "number" then
    format = format
  elseif format == "GL_RGBA" then
    format = GL_RGBA
  elseif format == "GL_RGBA8" then
    format = GL_RGBA8
  elseif format == "GL_RGBA16" then
    format = GL_RGBA16
  elseif format == "GL_RGBA16F" then
    format = GL_RGBA16F
  elseif format == "GL_RGBA16F_ARB" then
    format = GL_RGBA16F_ARB
  elseif format == "GL_RGBA32F" then
    format = GL_RGBA32F
  elseif format == "GL_RGBA32F_ARB" then
    format = GL_RGBA32F_ARB
  elseif format == "GL_RGB" then
    format = GL_RGB
  elseif format == "GL_RGB8" then
    format = GL_RGB8
  elseif format == "GL_RGB16" then
    format = GL_RGB16
  elseif format == "GL_RGB16F" then
    format = GL_RGB16F
  elseif format == "GL_RGB16F_ARB" then
    format = GL_RGB16F_ARB
  elseif format == "GL_RGB32F" then
    format = GL_RGB32F
  elseif format == "GL_RGB32F_ARB" then
    format = GL_RGB32F_ARB
  elseif format == "GL_RG" then
    format = GL_RG
  elseif format == "GL_RG8" then
    format = GL_RG8
  elseif format == "GL_RG16" then
    format = GL_RG16
  elseif format == "GL_RG16F" then
    format = GL_RG16F
  elseif format == "GL_RG32F" then
    format = GL_RG32F
  elseif format == "GL_R" then
    format = GL_R
  elseif format == "GL_R8" then
    format = GL_R8
  elseif format == "GL_R16" then
    format = GL_R16
  elseif format == "GL_R16F" then
    format = GL_R16F
  elseif format == "GL_R32F" then
    format = GL_R32F
  else
    log:error(objName .. " : unknown format : " .. format)
    return
  end
  M.fbo:allocate(width, height, format, numSamples)
end

function ofelia._clear()
  M.fbo:clear()
end

function ofelia.clearColorBuffer(av)
  if #av == 4 then
    M.fbo:clearColorBuffer(ofFloatColor(av[1], av[2], av[3], av[4]))
  elseif #av == 5 then
    M.fbo:clearColorBuffer(av[1], ofFloatColor(av[2], av[3], av[4], av[5]))
  else
    log:error(objName .. " : 'clearColorBuffer' requires 4 or 5 arguments")
  end
end

function ofelia.clearDepthBuffer(f)
  M.fbo:clearDepthBuffer(f)
end

function ofelia.clearStencilBuffer(i)
  M.fbo:clearStencilBuffer(i)
end

function ofelia.clearDepthStencilBuffer(av)
  M.fbo:clearDepthStencilBuffer(av[1], av[2])
end

function ofelia.setAnchorPercent(fv)
  M.fbo:setAnchorPercent(fv[1], fv[2])
end

function ofelia.setAnchorPoint(fv)
  M.fbo:setAnchorPoint(fv[1], fv[2])
end

function ofelia.resetAnchor()
  M.fbo:resetAnchor()
end

function ofelia.setDefaultTextureIndex(i)
  M.fbo:setDefaultTextureIndex(i)
end

function ofelia.setUseTexture(b)
  M.fbo:setUseTexture(b ~= 0)
end

function ofelia.readToPixels(av)
  if type(av) == "table" then
    M.fbo:readToPixels(av[1], av[2])
  else
    M.fbo:readToPixels(av)
  end
end

function ofelia.copyTo(p)
  M.fbo:copyTo(p)
end

function ofelia.flagDirty()
  M.fbo:flagDirty()
end

function ofelia.updateTexture(i)
  M.fbo:updateTexture(i)
end

function ofelia.createAndAttachTexture(iv)
  M.fbo:createAndAttachTexture(iv[1], iv[2])
end

function ofelia.attachTexture(av)
  M.fbo:attachTexture(av[1], av[2], av[3])
end

function ofelia.createAndAttachRenderbuffer(iv)
  outlet:outletAnything(0, {"createAndAttachRenderbuffer", M.fbo:createAndAttachRenderbuffer(iv[1], iv[2])})
end

function ofelia.createAndAttachDepthStencilTexture(iv)
  if #iv == 3 then
    M.fbo:createAndAttachDepthStencilTexture(iv[1], iv[2], iv[3])
  elseif #iv == 5 then
    M.fbo:createAndAttachDepthStencilTexture(iv[1], iv[2], iv[3], iv[4], iv[5])
  else
    log:error(objName .. " : 'createAndAttachDepthStencilTexture' requires 3 or 5 arguments")
  end
end

function ofelia.setActiveDrawBuffer(i)
  M.fbo:setActiveDrawBuffer(i)
end

function ofelia.setActiveDrawBuffers(iv)
  M.fbo:setActiveDrawBuffers(iv)
end

function ofelia.activateAllDrawBuffers()
  M.fbo:activateAllDrawBuffers()
end
