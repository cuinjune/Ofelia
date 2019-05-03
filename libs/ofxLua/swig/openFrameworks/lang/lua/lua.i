// Lua specific settings
// 2017 Dan Wilcox <danomatika@gmail.com>

%include "std_filesystem_path.i"

// ----- typemaps -----

// convert lua string to unsigned char buffer,
// args from lang in to C++ function
%typemap(in) (unsigned char *STRING, int LENGTH) {
   $2 = (int)lua_tonumber(L, $input+1);
   $1 = (unsigned char *)lua_tolstring(L, $input, (size_t *)&$2);
}

// convert lua string to char buffer,
// args from lang in to C++ function
%typemap(in) (char *STRING, size_t LENGTH) {
   $2 = (size_t)lua_tonumber(L, $input+1);
   $1 = (char *)lua_tolstring(L, $input, &$2);
}

// convert char buffer to lua string,
// arg returned from C++ function out to lang
%typemap(out) (const char *BUFFER) {
   lua_pushlstring(L, $1, arg1->size());
   SWIG_arg++;
}

// ----- begin/end keyword renames -----

// ignore these to silence Warning 314
%ignore ofBaseMaterial::begin;
%ignore ofBaseMaterial::end;

// ignore these to silence Warning 314
%ignore ofBaseGLRenderer::begin;
%ignore ofBaseGLRenderer::end;

// DIFF: ofFbo: (Lua) beginFbo() & endFbo() since "end" is a Lua keyword
%rename(beginFbo) ofFbo::begin;
%rename(endFbo) ofFbo::end;

// DIFF: ofCamera.h: (Lua) beginCamera() & endCamera() since "end" is a Lua keyword
%rename(beginCamera) ofCamera::begin;
%rename(endCamera) ofCamera::end;

// DIFF: ofMaterial: (Lua) beginMaterial() & endMaterial() since "end" is a Lua keyword
%rename(beginMaterial) ofMaterial::begin;
%rename(endMaterial) ofMaterial::end;

// DIFF: ofShader: (Lua) beginShader() & endShader() since "end" is a Lua keyword
%rename(beginShader) ofShader::begin;
%rename(endShader) ofShader::end;

// ignore end keywords, even though they are within nested classes which are
// effectively ignored by SWIG
%ignore ofPixels_::Line::end;
%ignore ofPixels_::Lines::end;
%ignore ofPixels_::ConstPixels::end;
%ignore ofPixels_::ConstLine::end;
%ignore ofPixels_::ConstLines::end;
%ignore ofPixels_::Pixels::end;

%ignore ofUnicode::range::end;

%ignore ofBuffer::Lines::end;
%ignore ofBuffer::RLines::end;

%ignore ofUTF8Iterator::end;

// ----- other -----

// DIFF: ofFbo: (Lua) ofFboMode | operator renamed to of.FboModeOr(m1, m2)
// DIFF: ofFbo: (Lua) ofFboMode & operator renamed to of.FboModeAnd(m1, m2)
%rename(FboModeOr) operator | (ofFboMode, ofFboMode);
%rename(FboModeAnd) operator & (ofFboMode, ofFboMode);

// DIFF: ofTrueTypeFont.h: (LUA) string static const strings
%rename(TTF_SANS) OF_TTF_SANS;
%rename(TTF_SERIF) OF_TTF_SERIF;
%rename(TTF_MONO) OF_TTF_MONO;

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
