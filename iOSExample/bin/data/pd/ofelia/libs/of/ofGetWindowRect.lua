function ofelia.bang()
  local rect = ofGetWindowRect();
  return {rect.x, rect.y, rect.width, rect.height}
end