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

#include "ofeliaString.h"
#include <cstring>

/* ________________________________________________________________________________
 * static variables
 */
const int ofeliaString::npos = -1;
char ofeliaString::temp_buff[MAX_LINE];

/* ________________________________________________________________________________
 * ofeliaString class methods
 */
char* ofeliaString::alloc(int size)
{
    char *ss = new char[size+1];
    return ss;
}

ofeliaString::ofeliaString(const char *ss)
{
    if (!ss) ss = "";
    m_length = static_cast<int>(strlen(ss));
    m_data = alloc(m_length);
    strcpy(m_data, ss);
}

ofeliaString::ofeliaString(int size)
{
    m_length = size;
    m_data = alloc(size);
    m_data[0] = 0;
}

ofeliaString::ofeliaString(const ofeliaString &s)
{
    m_length = s.m_length;
    m_data = alloc(m_length);
    strcpy(m_data, s.m_data);
}

ofeliaString::~ofeliaString()
{
    delete m_data;
    m_data = NULL;
}

ofeliaString& ofeliaString::operator=(const ofeliaString &s)
{
    copy(s.c_str(), s.length());
    return *this;
}

ofeliaString& ofeliaString::operator=(const char *str)
{
    if (!str) str = "";
    copy(str, static_cast<int>(strlen(str)));
    return *this;
}

int ofeliaString::compare(const ofeliaString &s) const
{
    if(!m_data || !s.m_data) return -1; else
        return strcmp(m_data, s.m_data);
}

int ofeliaString::compare(const char *s) const
{
    return strcmp(m_data, s);
}

int ofeliaString::compare(const ofeliaString &s, int pos) const
{
    if(!m_data || !s.m_data) return -1; else
        return strncmp(m_data, s.m_data, pos);
}

int ofeliaString::compare(const char *s, int pos) const
{
    return strncmp(m_data, s, pos);
}

ofeliaString ofeliaString::substr(int start, int n)
{
    int len = length();
    if (n == npos) n = len;
    if (start + n > len) n = len - start;
    ofeliaString T(n);
    const char * in = m_data + start;
    char * out = T.m_data;
    
    for (int i = 0; i < n; i++)
        *out++ = *in++;
    *out = 0;
    return T;
}

ofeliaString& ofeliaString::insert(int pos, char ch)
{
    int i, on = m_length;
    resize(++m_length);
    char *p = m_data + pos;
    
    for (i = on-pos; i >= 0; i--)
        p[i+1] = p[i];
    *p = ch;
    return *this;
}

ofeliaString& ofeliaString::insert(int pos, const char *s)
{
    int i, n = static_cast<int>(strlen(s)), on = m_length;
    m_length += n;
    resize(m_length);
    char *p = m_data + pos;
    
    for (i = on-pos; i >= 0; i--)
        p[i+n] = p[i];
    for (i = 0; i < n; i++)
        *p++ = *s++;
    return *this;
}

ofeliaString& ofeliaString::insert(int pos, const ofeliaString &s)
{
    return insert(pos, s.c_str());
}

ofeliaString& ofeliaString::erase(int pos, int len)
{
    if (len == npos)
        len = m_length - pos;
    int rest = m_length - len - pos;
    
    for (int i = 0; i < rest; i++)
        m_data[pos+i] = m_data[pos+len+i];
    m_length -= len;
    resize(m_length);
    m_data[m_length] = '\0';
    return *this;
}

ofeliaString& ofeliaString::replace(int pos, int len, const char *s)
{
    erase(pos, len);
    return insert(pos, s);
}

ofeliaString& ofeliaString::replace(int pos, int len, const ofeliaString &s)
{
    return replace(pos, len, s.c_str());
}

void ofeliaString::push_back(char ch)
{
    char tmp[] = {ch,'\0'};
    resize(++m_length);
    strcat(m_data, tmp);
}

void ofeliaString::pop_back()
{
    erase(m_length-1, 1);
}

void ofeliaString::clear()
{
    erase(0, npos);
}

typedef long long intptr;

int ofeliaString::offs(const char *ss)
{
    return static_cast<int>(!ss ? ofeliaString::npos : intptr(ss) - intptr(c_str()));
}

int ofeliaString::find(char ch)
{
    return offs(strchr(c_str(), ch));
}

int ofeliaString::find(const char *ps)
{
    return offs(strstr(c_str(), ps));
}

int ofeliaString::find(const ofeliaString &s)
{
    return find(s.c_str());
}

int ofeliaString::rfind(char ch)
{
    return offs(strrchr(c_str(), ch));
}

int ofeliaString::find_first_of(const char *ps, int pos)
{
    return offs(strpbrk(c_str() + pos, ps));
}

int ofeliaString::find_first_of(const ofeliaString &str, int pos)
{
    return find_first_of(str.c_str(), pos);
}

int ofeliaString::find_first_not_of(const char *ps, int pos)
{
    int sz = static_cast<int>(strspn(c_str() + pos, ps));
    
    if (sz + pos == m_length)
        return ofeliaString::npos;
    else
        return sz + pos;
}

int ofeliaString::find_first_not_of(const ofeliaString &str, int pos)
{
    return find_first_not_of(str.c_str(), pos);
}

ofeliaString & ofeliaString::operator+=(const ofeliaString &s)
{
    m_length += s.length();
    resize(m_length);
    strcat(m_data, s.m_data);
    return *this;
}

ofeliaString& ofeliaString::operator+=(const char *str)
{
    if (!str) str = "";
    m_length += static_cast<int>(strlen(str));
    resize(m_length);
    strcat(m_data, str);
    return *this;
}

ofeliaString& ofeliaString::operator+=(char ch)
{
    char tmp[] = {ch,'\0'};
    return (*this) += tmp;
}

void ofeliaString::copy(const char*str, int sz)
{
    if (sz > length() || sz == 0) resize(sz);
    m_length = sz;
    strcpy(m_data, str);
}

int ofeliaString::resize(int size)
{
    char * new_str = alloc(size);
    if (m_data) strncpy(new_str,m_data, size+1);
    if (m_length > 0) {delete m_data; m_data = NULL;}
    m_data = new_str;
    return size;
}

ofeliaString operator+(ofeliaString s1, ofeliaString s2)
{
    ofeliaString temps(s1);
    temps += s2;
    return temps;
}

std::ostream& operator<<(std::ostream &os, const ofeliaString &s)
{
    return os << s.c_str();
}

std::istream& operator>>(std::istream &is, ofeliaString &s)
{
    is >> ofeliaString::temp_buff;
    s = ofeliaString::temp_buff;
    return is;
}

std::istream& getline(std::istream &is, ofeliaString &str)
{
    is.getline(ofeliaString::temp_buff, sizeof(ofeliaString::temp_buff));
    str = ofeliaString::temp_buff;
    return is;
}

