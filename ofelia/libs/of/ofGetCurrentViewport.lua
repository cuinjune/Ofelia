function ofelia.bang()
  local rect = ofGetCurrentViewport()
  return {rect.x, rect.y, rect.width, rect.height}
end