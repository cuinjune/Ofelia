local m = of.Matrix4x4()

function ofelia.bang()
  of.loadMatrix(m:getPtr())
  return nil
end

function ofelia.m(p)
  m = p
end