local m = of.Matrix4x4()

function ofelia.bang()
  of.loadViewMatrix(m:getPtr())
  return nil
end

function ofelia.m(p)
  m = p
end