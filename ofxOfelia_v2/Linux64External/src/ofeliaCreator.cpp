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

#include "ofeliaCreator.h"
#include "ofeliaDefine.h"
#include "ofeliaGet.h"
#include "ofeliaSet.h"
#include "ofeliaExists.h"
#include <string.h>

void *ofeliaCreator::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    if (!argc || argv[0].a_type != A_SYMBOL)
        pd_this->pd_newest = static_cast<t_pd *>(ofeliaDefine::newWrapper(s, argc, argv));
    else
    {
        const char *str = argv[0].a_w.w_symbol->s_name;
        if (!strcmp(str, "d") || !strcmp(str, "define"))
            pd_this->pd_newest = static_cast<t_pd *>(ofeliaDefine::newWrapper(s, argc - 1, argv + 1));
        else if (!strcmp(str, "get"))
            pd_this->pd_newest = static_cast<t_pd *>(ofeliaGet::newWrapper(s, argc - 1, argv + 1));
        else if (!strcmp(str, "set"))
            pd_this->pd_newest = static_cast<t_pd *>(ofeliaSet::newWrapper(s, argc - 1, argv + 1));
        else if (!strcmp(str, "e") || !strcmp(str, "exists"))
            pd_this->pd_newest = static_cast<t_pd *>(ofeliaExists::newWrapper(s, argc - 1, argv + 1));
        else
        {
            error("list %s: unknown function", str);
            pd_this->pd_newest = 0;
        }
    }
    return pd_this->pd_newest;
}

void ofeliaCreator::setup()
{
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper), gensym("ofelia"), A_GIMME, 0);
}

