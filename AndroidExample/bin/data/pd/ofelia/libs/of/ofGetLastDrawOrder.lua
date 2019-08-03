function ofelia.bang()
  local t = ofWindow.getListenerData("draw")
  if #t == 0 then
    return nil
  end
  return t[#t].order
end