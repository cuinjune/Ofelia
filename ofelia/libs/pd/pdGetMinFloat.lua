local PD = pdPD()

function ofelia.bang()
  return PD:getMinFloat()
end