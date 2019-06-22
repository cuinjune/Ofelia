local osc = pdBlOsc()
local inlet = pdInlet(this)

function ofelia.float(f)
  inlet:setSignalInlet(f)
end

function ofelia.perform(s1)
  return osc:saw(s1)
end

function ofelia.setPhase(f)
  osc:setPhase(f)
end