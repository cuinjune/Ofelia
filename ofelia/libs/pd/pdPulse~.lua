local osc = pdOsc()
local inlet = pdInlet(this)

function ofelia.float(f)
  inlet:setSignalInlet(f)
end

function ofelia.perform(s1, s2)
  return osc:pulse(s1, s2)
end

function ofelia.setPhase(f)
  osc:setPhase(f)
end