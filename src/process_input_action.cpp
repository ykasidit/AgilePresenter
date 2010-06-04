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
#include "process_input_action.h"
#include "agileserver.h"

int process_input_action(QString request,QTcpSocket &socket)
{
    if(!AgileServer::curInstance)
        return -2;

    char command[1024];
    QString buffer;
    QString status = "OK";
    AgileServer::curInstance->get_page(buffer,status,1);

    if(request.startsWith("/click"))
    {
        if(AgileServer::curInstance->simulate_click() == 0)
            status = "Done Command:  Mouse Click";
        else
            status = "Mouse Click Failed";

        AgileServer::curInstance->get_page(buffer,status,1);
    }
    else
    {
        QString str_cmd = request;


        if(str_cmd.startsWith("/"))
            str_cmd = str_cmd.mid(1);

        if(str_cmd.endsWith("?") && str_cmd.length()>1)
            str_cmd = str_cmd.left(str_cmd.length()-1);

        if(AgileServer::curInstance->simulate_key(str_cmd.toAscii())==0)
        {
            //sprintf(command,"Done Command: %s",str_cmd.toAscii());
        }
        else
        {
            return -1;
            //sprintf(command,"Unknown Command: %s",str_cmd.toAscii());
        }
    }


    




     send_header(200, "text/html", buffer.size(), socket);
     socket.write(buffer.toAscii());

     return 0;
}
