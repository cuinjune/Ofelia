/*==============================================================================
	ofelia: OpenFrameworks as an External Library for Interactive Applications
 
	Copyright (c) 2018 Zack Lee <cuinjune@gmail.com>
 
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/cuinjune/ofxOfelia for documentation
 ==============================================================================*/

/*==============================================================================
	ofeliaString class is based on the work of Steve Donovan's tinycpp
	which is free open source software, licensed under the MIT License.
 
	Copyright (C) 2013 Steve Donovan.
 
	Permission is hereby granted, free of charge, to any person obtaining a
	copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:
 
	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.
 
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
	AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	
	See https://github.com/stevedonovan/tinycpp for documentation
 ==============================================================================*/

#ifndef ofeliaString_h
#define ofeliaString_h

#include <iostream>

#define MAX_LINE 512

class ofeliaString {
    
private:
    
    char *m_data;
    int m_length;
    
    int offs(const char *ss);
    char *alloc(int size);
    
public:
    
    ofeliaString(const char *str=0);
    ofeliaString(int size);
    ofeliaString(const ofeliaString &s);
    ofeliaString(const ofeliaString &s, int flags);
    ~ofeliaString();
    
    typedef char *iterator;
    iterator begin() {return m_data;}
    iterator end() {return m_data+m_length;}
    
    int length() const {return m_length;}
    int size() const {return m_length;}
    bool empty() const {return m_length==0;}
    const char *c_str() const {return m_data;}
    
    ofeliaString& operator=(const ofeliaString &s);
    ofeliaString& operator=(const char *str);
    char& operator[](int i) {return m_data[i];}
    char& front() {return m_data[0];}
    char& back() {return m_data[m_length-1];}
    
    void copy(const char *str, int sz);
    int compare(const ofeliaString &s) const;
    int compare(const char *s) const;
    int compare(const ofeliaString &s, int pos) const;
    int compare(const char *s, int pos) const;
    
    ofeliaString& operator+=(const ofeliaString &s);
    ofeliaString& operator+=(const char *str);
    ofeliaString& operator+=(char ch);
    
    ofeliaString substr(int start, int n=npos);
    int find(char ch);
    int find(const char *ps);
    int find(const ofeliaString &s);
    int rfind(char ch);
    int find_first_of(const char *ps, int pos);
    int find_first_of(const ofeliaString &str, int pos=0);
    int find_first_not_of(const char *ps, int pos);
    int find_first_not_of(const ofeliaString &str, int pos=0);
    
    ofeliaString& insert(int pos, char ch);
    ofeliaString& insert(int pos, const char *s);
    ofeliaString& insert(int pos, const ofeliaString &s);
    ofeliaString& erase(int pos=0, int len=npos);
    ofeliaString& replace(int pos, int len, const char *s);
    ofeliaString& replace(int pos, int len, const ofeliaString &s);
    
    void push_back(char ch);
    void pop_back();
    void clear();
    int resize(int size);
    static const int npos;
    static char temp_buff[MAX_LINE];
};

ofeliaString operator+(ofeliaString s1, ofeliaString s2);
inline int operator==(const ofeliaString &s1, const ofeliaString &s2) {return s1.compare(s2)==0;}
inline int operator!=(const ofeliaString &s1, const ofeliaString &s2) {return s1.compare(s2)!=0;}
inline int operator!=(const ofeliaString &s1, const char *ps2) {return s1.compare(ps2)!=0;}
inline int operator<(const ofeliaString &s1, const ofeliaString &s2) {return s1.compare(s2)<0;}
inline int operator>(ofeliaString &s1, ofeliaString &s2) {return s1.compare(s2)>0;}
std::ostream& operator<<(std::ostream&, const ofeliaString&);
std::istream& operator>>(std::istream&, ofeliaString&);
std::istream& getline(std::istream &is, ofeliaString &str);

#endif /* ofeliaString_h */
