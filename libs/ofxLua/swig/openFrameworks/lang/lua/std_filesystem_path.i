// std::filesystem::path wrapper to convert to Lua strings automatically
// adapted from SWIG lua/std_string.i
// 2017 Dan Wilcox <danomatika@gmail.com>

%{
#include <boost/filesystem.hpp>
%}

namespace std {
namespace filesystem {

%naturalvar path;

// Lua strings and std::string can contain embedded zero bytes
// Therefore a standard out typemap should not be:
//     lua_pushstring(L, $1.c_str());
// but
//     lua_pushlstring(L, $1.data(), $1.size());
//
// Similarly for getting the string
//     $1 = (char*)lua_tostring(L, $input);
// becomes
//     size_t len = lua_rawlen(L, $input);
//     $1 = lua_tolstring(L, $input, &len);
//  
// Not using: lua_tolstring() as this is only found in Lua 5.1 & not 5.0.2

%typemap(in, checkfn="lua_isstring") path {
	size_t len = lua_rawlen(L, $input);
	$1 = lua_tolstring(L, $input, &len);
}

%typemap(out) path {
	lua_pushlstring(L, $1.string().data(), $1.string().size());
	SWIG_arg++;
}

%typemap(in, checkfn="lua_isstring") const path& ($*1_ltype temp) {
	size_t len = lua_rawlen(L, $input);
	temp = lua_tolstring(L, $input, &len);
	$1 = &temp;
}

%typemap(out) const path& {
	lua_pushlstring(L, $1->data(), $1->size());
	SWIG_arg++;
}

// for throwing of any kind of path, path ref's and path pointers
// we convert all to lua strings
%typemap(throws) path, path&, const path& {
	lua_pushlstring(L, $1.data(), $1.size());
	SWIG_fail;
}

%typemap(throws) path*, const path* {
	lua_pushlstring(L, $1->data(), $1->size());
	SWIG_fail;
}

%typecheck(SWIG_TYPECHECK_STRING) path, const path& {
	$1 = lua_isstring(L, $input);
}

%typemap(in) string &INPUT = const string &;

%typemap(in, numinputs=0) string &OUTPUT ($*1_ltype temp) {
	$1 = &temp;
}

%typemap(argout) string &OUTPUT {
	lua_pushlstring(L, $1->data(), $1->size());
	SWIG_arg++;
}

%typemap(in) string &INOUT = const string &;

%typemap(argout) string &INOUT = string &OUTPUT;

// a really cut down version of the std::filesystem::path class
// this provides basic mapping of lua strings <-> std::filesystem::path
// and little else
class path {
	public:
		path();
		path(const char*);
		string string() const;
		// no support for all the other features
		// it's probably better to do it in lua
};

} // filesystem
} // std
