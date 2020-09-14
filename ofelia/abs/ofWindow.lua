M.window = ofWindow()

function M.bang()
  return M.window
end

function M.free()
  M.window:destroy()
end

function M.setGLVersion(iv)
  M.window:setGLVersion(iv[1], iv[2])
end

function M.setGLESVersion(i)
  M.window:setGLESVersion(i)
end

function M.setPosition(iv)
  M.window:setPosition(iv[1], iv[2])
end

function M.setSize(iv)
  M.window:setSize(iv[1], iv[2])
end

function M.create()
  M.window:create()
end

function M.destroy()
  M.window:destroy()
end

function M.glVersionMajor(i)
  M.window.glVersionMajor = i
end

function M.glVersionMinor(i)
  M.window.glVersionMinor = i
end

function M.glesVersion(i)
  M.window.glesVersion = i
end

function M.title(s)
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