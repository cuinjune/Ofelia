// Python specific settings
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- operator overloads -----

%rename(__getitem__) *::operator[];
%rename(__mul__) *::operator*;
%rename(__div__) *::operator/;
%rename(__add__) *::operator+;
%rename(__sub__) *::operator-;
