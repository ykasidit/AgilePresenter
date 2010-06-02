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
#include "agileserver.h"
#include "agilethread.h"
#include "main_template.h"

#ifdef  Q_WS_WIN
#define _WIN32_WINNT 0x0500 /*otherwise INPUT wont be defined*/

static const int KModMaskCtrl = 1;
static const int KModMaskAlt = 2;
static const int KModMaskShift = 4;

#include <windows.h>
#include <Winuser.h>
#include <Wtsapi32.h>
#include <sys/types.h>
#include <shellapi.h>
#endif

#include <QDesktopWidget>
#include <QApplication>
#include <QFile>
#include "avail_commands.h"
#include <QFileInfo>
#include <QMessageBox>

AgileServer* AgileServer::curInstance = NULL;

void AgileServer::incomingConnection(int socketDescriptor)
{
    AgileThread *thread = new AgileThread(socketDescriptor);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    emit on_state("new req");
    curInstance = this;
}

AgileServer::AgileServer()
{
    QString abs_path = QFileInfo( QCoreApplication::argv()[0] ).absolutePath();
    abs_path += "/public_html";
    abs_path += "/main.html";

    QFile file(abs_path);

    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray bytes = file.readAll();
        file.close();
        main_template = bytes;
        qDebug(main_template.toAscii());
    }
    else
    {
        
        qDebug("read main.html failed");
        QString err;
        err = tr("Failed to read main.html in public_html folder: ");
        err += abs_path;
        QMessageBox::critical(NULL, tr("AgilePresenter"), err);
    }
}



int AgileServer::simulate_key(const char* key_str)
{


    int code=0;
    int mod = 0; //TODO: implement modifiers
    int ret = get_key_code(key_str,&code);
    if(ret != 0)
        return ret;




    m_mutex.lock();

    qDebug("gen_key %s",key_str);

        #ifdef  Q_WS_WIN
        //simulate on win32
        INPUT  Input={0};
        if(mod & KModMaskCtrl)
        {
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type      = INPUT_KEYBOARD;
        Input.ki.wVk = VK_CONTROL;
        Input.ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1,&Input,sizeof(INPUT));
        Sleep(20);
        }

        if(mod & KModMaskAlt)
        {
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type      = INPUT_KEYBOARD;
        Input.ki.wVk = VK_MENU;
        Input.ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1,&Input,sizeof(INPUT));
        Sleep(20);
        }

        if(mod & KModMaskShift)
        {
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type      = INPUT_KEYBOARD;
        Input.ki.wVk = VK_SHIFT;
        Input.ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1,&Input,sizeof(INPUT));
        Sleep(20);
        }

        ZeroMemory(&Input,sizeof(INPUT));
        Input.type      = INPUT_KEYBOARD;
        Input.ki.wVk = code;
        Input.ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1,&Input,sizeof(INPUT));

        Sleep(20);
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type      = INPUT_KEYBOARD;
        Input.ki.wVk = code;
        Input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1,&Input,sizeof(INPUT));




        if(mod & KModMaskShift)
        {
        Sleep(20);
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type      = INPUT_KEYBOARD;
        Input.ki.wVk = VK_SHIFT;
        Input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1,&Input,sizeof(INPUT));

        }



        if(mod & KModMaskAlt)
        {
        Sleep(20);
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type      = INPUT_KEYBOARD;
        Input.ki.wVk = VK_MENU;
        Input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1,&Input,sizeof(INPUT));

        }

        if(mod & KModMaskCtrl)
        {
        Sleep(20);
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type      = INPUT_KEYBOARD;
        Input.ki.wVk = VK_CONTROL;
        Input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1,&Input,sizeof(INPUT));

        }
        #else
        //TODO: simulate in gnu_linux
        #endif

        m_mutex.unlock();

     return 0;
}

int AgileServer::simulate_click()
{
    QMutexLocker locker(&m_mutex);

    #ifdef Q_WS_WIN

        INPUT    Input={0};
        // left down
        Input.type      = INPUT_MOUSE;
        Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
        ::SendInput(1,&Input,sizeof(INPUT));

        Sleep(10);
        // left up
        ::ZeroMemory(&Input,sizeof(INPUT));
        Input.type      = INPUT_MOUSE;
        Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
        ::SendInput(1,&Input,sizeof(INPUT));

      #else
      //TODO: similate click in gnu_linux
      #endif

   return 0;
}

void AgileServer::request_ss()
{
    m_mutex.lock();
    ss = QPixmap::grabWindow(QApplication::desktop()->winId());    
    m_mutex.unlock();    

    qDebug("emit ss_ready");
    emit ss_ready();
}

QPixmap AgileServer::read_ss()
{
  QPixmap tmp;
  m_mutex.lock();
  tmp = ss;
  m_mutex.unlock();
  return tmp;
}

