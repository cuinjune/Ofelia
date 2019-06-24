// glm constants bindings
// adapted from https://github.com/IndiumGames/swig-wrapper-glm
//
// The MIT License (MIT)
//
// Copyright (c) 2016 Indium Games (www.indiumgames.fi)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// ----- glm/gtc/constants.hpp -----

template<typename T> T epsilon();
template<typename T> T zero();
template<typename T> T one();
template<typename T> T pi();
template<typename T> T two_pi();
template<typename T> T root_pi();
template<typename T> T half_pi();
template<typename T> T three_over_two_pi();
template<typename T> T quarter_pi();
template<typename T> T one_over_pi();
template<typename T> T one_over_two_pi();
template<typename T> T two_over_pi();
template<typename T> T four_over_pi();
template<typename T> T two_over_root_pi();
template<typename T> T one_over_root_two();
template<typename T> T root_half_pi();
template<typename T> T root_two_pi();
template<typename T> T root_ln_four();
template<typename T> T e();
template<typename T> T euler();
template<typename T> T root_two();
template<typename T> T root_three();
template<typename T> T root_five();
template<typename T> T ln_two();
template<typename T> T ln_ten();
template<typename T> T ln_ln_two();
template<typename T> T third();
template<typename T> T two_thirds();
template<typename T> T golden_ratio();

%template(epsilon)           epsilon<float>;
%template(zero)              zero<float>;
%template(one)               one<float>;
%template(pi)                pi<float>;
%template(root_pi)           root_pi<float>;
%template(half_pi)           half_pi<float>;
%template(quarter_pi)        quarter_pi<float>;
%template(one_over_pi)       one_over_pi<float>;
%template(two_over_pi)       two_over_pi<float>;
%template(two_over_root_pi)  two_over_root_pi<float>;
%template(one_over_root_two) one_over_root_two<float>;
%template(root_half_pi)      root_half_pi<float>;
%template(root_two_pi)       root_two_pi<float>;
%template(root_ln_four)      root_ln_four<float>;
%template(e)                 e<float>;
%template(euler)             euler<float>;
%template(root_two)          root_two<float>;
%template(root_three)        root_three<float>;
%template(root_five)         root_five<float>;
%template(ln_two)            ln_two<float>;
%template(ln_ten)            ln_ten<float>;
%template(ln_ln_two)         ln_ln_two<float>;
%template(third)             third<float>;
%template(two_thirds)        two_thirds<float>;
%template(golden_ratio)      golden_ratio<float>;
