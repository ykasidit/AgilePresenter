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
#include "process_helpers.h"
#include "main_template.h"

void send_header(int httperr, const char* contenttype, int contentlen, QTcpSocket& socket)
{
    char header[2048];

    sprintf(header, headerformat, httperr,contenttype, contentlen );
    socket.write(header);    
}

