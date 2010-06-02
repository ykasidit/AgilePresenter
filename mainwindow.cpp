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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QNetworkInterface>

//this app is mainly based on qt threaded fortune server example code

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->iURL->setText("Please wait...");
    if (!server.listen(QHostAddress::Any,80))

    {
        if(!server.listen())
        {
            QMessageBox::critical(this, tr("AgilePresenter"),
                              tr("Unable to start the server: %1.")
                              .arg(server.errorString()));

            close();
            exit(1);
            return;
        }

    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    
    if(server.serverPort() == 80)
    {
        ui->iURL->setText(tr("http://%1")
                             .arg(ipAddress));
    }
    else
    {
    ui->iURL->setText(tr("http://%1:%2")
                         .arg(ipAddress).arg(server.serverPort()));
    }
    ui->statusBar->showMessage("AgilePresenter Server running");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
