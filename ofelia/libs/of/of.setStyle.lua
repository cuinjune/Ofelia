local style = of.Style()

function ofelia.bang()
  of.setStyle(style)
  return nil
end

function ofelia.style(p)
  style = p
end