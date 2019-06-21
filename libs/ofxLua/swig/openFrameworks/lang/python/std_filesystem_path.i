// std::filesystem::path wrapper to convert to Python strings automatically
// adapted from SWIG typemaps/std_string.swg
// 2017 Dan Wilcox <danomatika@gmail.com>

%include <typemaps/std_strings.swg>

%fragment("<string>");

namespace std {
namespace filesystem {
	%naturalvar path;
	class path;
} // filesystem
} // std

%typemaps_std_string(std::filesystem::path, char, SWIG_AsCharPtrAndSize, SWIG_FromCharPtrAndSize, %checkcode(STDSTRING));
