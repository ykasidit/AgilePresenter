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
#include "agilethread.h"
#include <QTextStream>
#include <QStringList>
#include <QDateTime>
#include "main_template.h"
#include "process_index.h"
#include "process_input_action.h"
#include "process_get_img.h"

AgileThread::AgileThread(int socketDescriptor)
{
    this->socketDescriptor = socketDescriptor;
}


int process_req(QString& request,QTcpSocket &socket)
{   
   qDebug(request.toAscii());


   if(request == "/" || request.startsWith("/?") || request.startsWith("/refresh") || request.length() == 0)
   {       
        process_index(request,socket);
   }      
   else if( process_input_action(request,socket) == 0)
   {
        ;
   }
   else
   {
       process_get_img* pgi = new process_get_img();

       if( pgi->process(request,socket) == 0)
            ;
       else
       {
           int httperr=404;
           char* contenttype = "text/plain";
           char* content = "Sorry - not found on this AgilePresenter server";
           long contentlen = strlen(content);

           send_header(httperr,contenttype, contentlen, socket);
           socket.write(content);
       }

       delete pgi;
   }

   return 0;

}


//see qt example fortune server...

void AgileThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    tcpSocket.waitForReadyRead(2000);    
    QString reqdoc;

    QByteArray read;
    read = tcpSocket.readAll();
    QString reqstr(read);
    int pos=-1;
    pos = reqstr.indexOf("GET");
    if(pos>=0)
    {
        QString getdoc = reqstr.mid(pos);
        QStringList list = getdoc.split(" ");
        if(list.size()>1)
           reqdoc = list[1];
    }

    QString out;

    //determine what to return from reqdoc
    int ret = process_req(reqdoc,tcpSocket);

    if(ret != 0)
    {
        int httperr=404;
        char header[2048];
        char* headerformat = "HTTP/1.1 %d OK\r\nserver: AgilePresenter-1.0.0\r\ncontent-type: %s\r\ncontent-length: %ld\r\nConnection: close\r\n\r\n";
        char* contenttype = "text/plain";
        long contentlen = 0;
        char* content = "Sorry - not found";
        contentlen = strlen(content);

        sprintf(header, headerformat, httperr,contenttype, contentlen );
        qDebug(header);
        tcpSocket.write(header);
        tcpSocket.write(content);
    }

    tcpSocket.flush();
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}
