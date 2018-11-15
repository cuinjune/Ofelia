local window = pd.OFWindow()

function ofelia.free()
  window:destroy()
end

function ofelia.setGLVersion(iv)
  window:setGLVersion(iv[1], iv[2])
end

function ofelia.getPosition(iv)
  return window:getPosition()
end

function ofelia.setPosition(iv)
  window:setPosition(iv[1], iv[2])
end

function ofelia.isPositionSet()
  return window:isPositionSet()
end

function ofelia.getWidth()
  return window:getWidth()
end

function ofelia.getHeight()
  return window:getHeight()
end

function ofelia.setSize(iv)
  window:setSize(iv[1], iv[2])
end

function ofelia.isSizeSet()
  return window:isSizeSet()
end

function ofelia.create()
  window:create()
end

function ofelia.destroy()
  window:destroy()
end

function ofelia.glVersionMajor(i)
  window.glVersionMajor = i
end

function ofelia.glVersionMinor(i)
  window.glVersionMinor = i
end

function ofelia.title(s)
  window.title = s
end

function ofelia.windowMode(i)
  window.windowMode = i
end

function ofelia.numSamples(i)
  window.numSamples = i
end

function ofelia.doubleBuffering(b)
  window.doubleBuffering = b ~= 0
end

function ofelia.redBits(i)
  window.redBits = i
end

function ofelia.greenBits(i)
  window.greenBits = i
end

function ofelia.blueBits(i)
  window.blueBits = i
end

function ofelia.alphaBits(i)
  window.alphaBits = i
end

function ofelia.depthBits(i)
  window.depthBits = i
end

function ofelia.stencilBits(i)
  window.stencilBits = i
end

function ofelia.stereo(b)
  window.stereo = b ~= 0
end

function ofelia.visible(b)
  window.visible = b ~= 0
end

function ofelia.iconified(b)
  window.iconified = b ~= 0
end

function ofelia.decorated(b)
  window.decorated = b ~= 0
end

function ofelia.resizable(b)
  window.resizable = b ~= 0
end

function ofelia.monitor(i)
  window.monitor = i
end

function ofelia.multiMonitorFullScreen(b)
  window.multiMonitorFullScreen = b ~= 0
end