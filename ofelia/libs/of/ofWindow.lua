M.window = ofWindow()

function ofelia.bang()
  return M.window
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
  M.window:destroy()
end

function ofelia.setGLVersion(iv)
  M.window:setGLVersion(iv[1], iv[2])
end

function ofelia.setPosition(iv)
  M.window:setPosition(iv[1], iv[2])
end

function ofelia.setSize(iv)
  M.window:setSize(iv[1], iv[2])
end

function ofelia.create()
  M.window:create()
end

function ofelia.destroy()
  M.window:destroy()
end

function ofelia.glVersionMajor(i)
  M.window.glVersionMajor = i
end

function ofelia.glVersionMinor(i)
  M.window.glVersionMinor = i
end

function ofelia.title(s)
  M.window.title = s
end

function M.windowMode(a)
  if type(a) == "number" then
    M.window.windowMode = a
  elseif a == "OF_WINDOW" then
    M.window.windowMode = 0
  elseif a == "OF_FULLSCREEN" then
    M.window.windowMode = 1
  elseif a == "OF_GAME_MODE" then
    M.window.windowMode = 2
  else
    log:error(objName .. " : unknown mode : " .. a)
  end
end

function ofelia.numSamples(i)
  M.window.numSamples = i
end

function ofelia.doubleBuffering(b)
  M.window.doubleBuffering = b ~= 0
end

function ofelia.redBits(i)
  M.window.redBits = i
end

function ofelia.greenBits(i)
  M.window.greenBits = i
end

function ofelia.blueBits(i)
  M.window.blueBits = i
end

function ofelia.alphaBits(i)
  M.window.alphaBits = i
end

function ofelia.depthBits(i)
  M.window.depthBits = i
end

function ofelia.stencilBits(i)
  M.window.stencilBits = i
end

function ofelia.stereo(b)
  M.window.stereo = b ~= 0
end

function ofelia.visible(b)
  M.window.visible = b ~= 0
end

function ofelia.iconified(b)
  M.window.iconified = b ~= 0
end

function ofelia.decorated(b)
  M.window.decorated = b ~= 0
end

function ofelia.resizable(b)
  M.window.resizable = b ~= 0
end

function ofelia.monitor(i)
  M.window.monitor = i
end

function ofelia.multiMonitorFullScreen(b)
  M.window.multiMonitorFullScreen = b ~= 0
end