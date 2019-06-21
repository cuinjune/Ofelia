local filter = pdFilter()

function ofelia.perform(s1, s2, s3, s4)
  return filter:lowShelf(s1, s2, s3, s4)
end