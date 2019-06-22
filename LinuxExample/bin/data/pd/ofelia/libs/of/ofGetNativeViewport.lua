function ofelia.bang()
  local rect = ofGetNativeViewport()
  return {rect.x, rect.y, rect.width, rect.height}
end