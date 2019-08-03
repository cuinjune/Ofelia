local canvas = pdCanvas(this)
local args = canvas:getArgs()
if #args == 0 then
  args[1] = "a"
end
table.insert(args, 1, "#a1 do local a = a1[i] a =")
table.insert(args, 1, ",")
table.insert(args, 1, "for i=1")
table.insert(args, 1, "__set")
table.insert(args, "a1[i] = a end return a1")
local send = pdSend(this)
send:sendAnything({"__clear"})
send:sendAnything(args)