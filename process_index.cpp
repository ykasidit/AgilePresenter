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
#include "process_index.h"

int process_index(QString& request,QTcpSocket &socket)
{
    QDateTime dt = QDateTime::currentDateTime();
    time_t now = dt.toTime_t();
    QString buffer;

    if(request.startsWith("/refresh"))
    {
        QString command = "Done Command: Refresh Screenshot";
        //mainpage: %s %d %d %ld
        AgileServer::curInstance->get_page(buffer,command,now);
    }
    else
    {
        QString msg("Welcome to AgilePresenter");
        AgileServer::curInstance->get_page(buffer,msg,now);
    }

    send_header(200, "text/html",buffer.size(), socket);
    qDebug(buffer.toAscii());
    socket.write(buffer.toAscii());

    return 0;
}
