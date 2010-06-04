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

#ifndef AGILESERVER_H
#define AGILESERVER_H

#include <QTcpServer>
#include <QMutex>
#include <QPixmap>

class AgileServer : public QTcpServer
{
    Q_OBJECT

    signals:
    void on_state(QString str);
    void ss_ready();

    
    public slots:
    void request_ss(); //req this and then call read_ss - mutexed so it's blocking until ss ready..,


public:
    AgileServer();
    static AgileServer* curInstance;
    int simulate_key(const char* key_str);
    int simulate_click();    
    QPixmap read_ss();
    QString main_template;

    void get_page(QString& ret, QString& status, long time)
    {
        //  [agile_presenter_status]
                  //[agile_presenter_time_num]
        ret = main_template;
        ret = ret.replace("[agile_presenter_status]",status);
        QString times;
        times.sprintf("%ld",time);
        ret = ret.replace("[agile_presenter_time_num]",times);

    }

protected:
    void incomingConnection(int socketDescriptor);
    QMutex m_mutex;
    QPixmap ss;
};

#endif // AGILESERVER_H
