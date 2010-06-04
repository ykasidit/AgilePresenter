/*
    Copyright (C) 2010 Kasidit Yusuf.

    This file is part of AgilePresenter.

    AgilePresenter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AgilePresenter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with AgilePresenter.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AVAIL_COMMANDS_H
#define AVAIL_COMMANDS_H

#include <QtGlobal>


#ifdef Q_WS_WIN
#include <Winuser.h>
#endif

typedef struct
{        
        char* name;
        int key_code;
} key_table_entry;

#ifdef Q_WS_WIN
static const key_table_entry win_key_map[] = {
    {"prev",VK_PRIOR},
    {"next",VK_NEXT},
    {"page_up",VK_PRIOR},
    {"page_down",VK_NEXT},
    {"home",VK_HOME},
    {"end",VK_END}
};

//return 0 if success
int get_key_code(const char* str, int* code)
{
        int table_len = sizeof(win_key_map)/sizeof(key_table_entry);
        int i=0;
        for(;i<table_len;i++)
        {
                if( strcmp(win_key_map[i].name,str) == 0)
                {
                        *code = win_key_map[i].key_code;
                        return 0;
                }
        }

        //not found

        return -1;
};

#endif

#endif // AVAIL_COMMANDS_H
