function ofelia.bang()
  local color = ofGetGlobalAmbientColor()
  return {color.r, color.g, color.b, color.a}
end