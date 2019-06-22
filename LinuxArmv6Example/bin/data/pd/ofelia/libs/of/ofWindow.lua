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

function ofelia.setGLESVersion(i)
  M.window:setGLESVersion(i)
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

function ofelia.glesVersion(i)
  M.window.glesVersion = i
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
    log:error("unknown mode : " .. a)
  end
end