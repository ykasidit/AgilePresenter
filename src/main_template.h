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
#ifndef MAIN_TEMPLATE_H
#define MAIN_TEMPLATE_H

#include <QString>


static const char* headerformat = "HTTP/1.1 %d OK\r\nserver: AgilePresenter-1.0.0\r\ncontent-type: %s\r\ncontent-length: %ld\r\nConnection: close\r\n\r\n";

static const int KPageBuffSize = 2048;

#endif // MAIN_TEMPLATE_H
