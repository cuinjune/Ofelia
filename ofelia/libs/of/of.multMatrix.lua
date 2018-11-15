local m = of.Matrix4x4()

function ofelia.bang()
  of.multMatrix(m:getPtr())
  return nil
end

function ofelia.m(p)
  m = p
end