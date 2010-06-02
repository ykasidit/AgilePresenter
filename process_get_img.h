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
#ifndef PROCESS_GET_IMG_H
#define PROCESS_GET_IMG_H

#include "process_helpers.h"
#include <QWaitCondition>
#include <QMutex>

class process_get_img : public QObject
{
    Q_OBJECT       

signals:
    void request_agileserver_ss();
public slots:
    void agileserver_ss_ready();

public:
    int process(QString& request,QTcpSocket &socket);
protected:
    QWaitCondition m_wait;
    QMutex m_mutex;//just for QWaitCondition
};

#endif // PROCESS_GET_IMG_H
