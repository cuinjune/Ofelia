local outlet = pdOutlet(this)
M.oscReceiver = pdOscReceiver(this)

function ofelia.free()
  M.oscReceiver:stop()
  M.oscReceiver = nil
end

function ofelia.setup(i)
  outlet:outletAnything(0, {"setup", M.oscReceiver:setup(i)})
end

function ofelia.start()
  outlet:outletAnything(0, {"start", M.oscReceiver:start()})
end

function ofelia.stop()
  M.oscReceiver:stop()
end

function ofelia.setPollingInterval(f)
  M.oscReceiver:setPollingInterval(f)
end

function ofelia.isListening()
  outlet:outletAnything(0, {"isListening", M.oscReceiver:isListening()})
end

function ofelia.hasWaitingMessages()
  outlet:outletAnything(0, {"hasWaitingMessages", M.oscReceiver:hasWaitingMessages()})
end

function ofelia.getPort()
  outlet:outletAnything(0, {"getPort", M.oscReceiver:getPort()})
end

function ofelia.received(av)
  table.insert(av, 1, "received")
  outlet:outletAnything(0, av)
end