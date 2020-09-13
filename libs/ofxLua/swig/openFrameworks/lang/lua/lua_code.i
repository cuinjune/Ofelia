// Lua specific code
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- luacode -----

// support for simple classes from http://lua-users.org/wiki/SimpleLuaClasses
//
// example usage:
//
// -- class declaration
// MyClass = class()
//
// -- constructor & attributes
// function MyClass:__init(x, y)
//     self.x = x
//     self.y = y
//     self.bBeingDragged = false
//     self.bOver = false
//     self.radius = 4
// end
//
// -- create instance & access attribute
// myclass = MyClass(10, 10)
// myclass.x = 100

%luacode %{

-- this isn't wrapped correctly, so set it here
of.CLOSE = true

-- handle typedefs which swig doesn't wrap
of.Point = of.Vec3f

-- class.lua
-- Compatible with Lua 5.1 (not 5.0).
function class(base, __init)
   local c = {}    -- a new class instance
   if not __init and type(base) == 'function' then
	  __init = base
	  base = nil
   elseif type(base) == 'table' then
	-- our new class is a shallow copy of the base class!
	  for i,v in pairs(base) do
		 c[i] = v
	  end
	  c._base = base
   end
   -- the class will be the metatable for all its objects,
   -- and they will look up their methods in it.
   c.__index = c

   -- expose a constructor which can be called by <classname>(<args>)
   local mt = {}
   mt.__call = function(class_tbl, ...)
   local obj = {}
   setmetatable(obj,c)
   if class_tbl.__init then
	  class_tbl.__init(obj,...)
   else
	  -- make sure that any stuff from the base class is initialized!
	  if base and base.__init then
	  base.__init(obj, ...)
	  end
   end
   return obj
   end
   c.__init = __init
   c.is_a = function(self, klass)
	  local m = getmetatable(self)
	  while m do
		 if m == klass then return true end
		 m = m._base
	  end
	  return false
   end
   setmetatable(c, mt)
   return c
end

%}
