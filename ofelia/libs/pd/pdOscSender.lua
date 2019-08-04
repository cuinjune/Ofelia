local outlet = pdOutlet(this)
M.oscSender = pdOscSender()

function ofelia.free()
  M.oscSender:clear()
  M.oscSender = nil
end

function ofelia.setup(av)
  outlet:outletAnything(0, {"setup", M.oscSender:setup(av[1], av[2])})
end

function ofelia.clear()
  M.oscSender:clear()
end

function ofelia.send(av)
  M.oscSender:send(av)
end

function ofelia.getHost()
  outlet:outletAnything(0, {"getHost", M.oscSender:getHost()})
end

function ofelia.getPort()
  outlet:outletAnything(0, {"getPort", M.oscSender:getPort()})
end