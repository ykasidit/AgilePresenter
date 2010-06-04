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
#include "process_get_img.h"
#include <QPixmap>
#include <QBuffer>
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include "agileserver.h"

void process_get_img::agileserver_ss_ready()
{
    qDebug("got agileserver_ss_ready");
    m_wait.wakeAll();
}
int process_get_img::process(QString& request,QTcpSocket &socket)
{
    char command[128];
    char buffer[KPageBuffSize];
    QDateTime dt = QDateTime::currentDateTime();
    time_t now = dt.toTime_t();

    if(request.startsWith("/getss") )
    {
        if(!AgileServer::curInstance)
            return -2;

        QObject::connect(this,SIGNAL(request_agileserver_ss(void)),AgileServer::curInstance,SLOT(request_ss(void)));
        QObject::connect(AgileServer::curInstance,SIGNAL(ss_ready(void)),this,SLOT(agileserver_ss_ready(void)));

        m_mutex.lock();        
        qDebug("emit request_agileserver_ss");
        emit request_agileserver_ss();
        qDebug("wait");
        m_wait.wait(&m_mutex,300);
        qDebug("wait end");
        m_mutex.unlock();

        QPixmap pixmap = AgileServer::curInstance->read_ss();

        QByteArray bytes;
        QBuffer buffer(&bytes);
        if(buffer.open(QIODevice::WriteOnly))
        {
            if(pixmap.save(&buffer, "JPG"))
            {
            qDebug("sending ss size %d",bytes.size());
            send_header(200, "image/jpeg", bytes.size(), socket);
            socket.write(bytes);
            }
            else
            {
                    const char* errstr = "Failed to convert screenshot to JPG buffer";
                    send_header(200, "text/plain", strlen(errstr), socket);
                    socket.write(errstr);
            }
        }
        else
        {
            const char* errstr = "Failed to open buffer";
            send_header(200, "text/plain", strlen(errstr), socket);
            socket.write(errstr);
        }
    }
    else if(request.contains(".") )
    {
        QString abs_path = QFileInfo( QCoreApplication::argv()[0] ).absolutePath();
        abs_path += "/public_html";
        abs_path += request;

        QFile file(abs_path);

        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray bytes = file.readAll();
            file.close();
            char* contenttype = "application/octet-stream";

            if(request.contains(".gif"))
                contenttype = "image/gif";
            else
            if(request.contains(".jpg"))
                contenttype = "image/jpeg";
            else
            if(request.contains(".css"))
                contenttype = "text/css";
            else
            if(request.contains(".html"))
                contenttype = "text/html";

            send_header(200, contenttype, bytes.size(), socket);
            socket.write(bytes);

        }
        else
        {
        const char* errstr = "File not found - make sure you refer to a file in public_html folder, in AgilePresenter folder.";
        send_header(404, "text/plain", strlen(errstr), socket);
        socket.write(errstr);
        }
    }
    else
        return -1;    

    return 0;
}



