/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <sailfishapp.h>
#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QDirIterator>
#include <QListWidget>
#include <QAction>
#include <QMenu>
#include <QAbstractListModel>
#include "custommenuitem.h"
#include <QtQuick>
#include <string>
#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "common/rhoparams.h"
#include "common/StringConverter.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoFilePath.h"
#include <qglobal.h>
#include <QMessageBox>
#include <QDir>
#include "impl/MainWindowImpl.h"
#include "QtLogView.h"
#include "../../platform/shared/qt/rhodes/RhoSimulator.h"

using namespace rho;
using namespace rho::common;
using namespace std;

static String g_strCmdLine;

static bool m_isJSApplication = false;
static String m_strRootPath, m_strRhodesPath, m_logPort;
static String m_strHttpProxy;

extern void parseHttpProxyURI(const String &http_proxy);
extern "C" {
    void rho_ringtone_manager_stop();

    const char* rho_native_rhopath()
    {
        return m_strRootPath.c_str();
    }
}

namespace rho
{
    //TODO: include intents to Win RhoSIm
    void waitIntentEvent(const rho::String& appName){}
}

char* parseToken(const char* start)
{
    int len = strlen(start);
    int nNameLen = 0;
    while (*start==' ') { start++; len--; }

    int i = 0;
    for (i = 0; i < len; i++) {
        if (start[i] == '=') {
            if (i > 0) {
                int s = i-1;
                for (; s >= 0 && start[s]==' '; s--);
                nNameLen = s+1;
                break;
            } else
                break;
        }
    }

    if ( nNameLen == 0 )
        return NULL;

    const char* szValue = start + i+1;
    int nValueLen = 0;

    while (*szValue==' ' || *szValue=='\'' || *szValue=='"' && nValueLen >= 0) { szValue++; }
    while (szValue[nValueLen] && szValue[nValueLen] !='\'' && szValue[nValueLen] != '"') { nValueLen++; }

    //while (nValueLen > 0 && (szValue[nValueLen-1]==' ' || szValue[nValueLen-1]=='\'' || szValue[nValueLen-1]=='"')) nValueLen--;

    char* value = (char*) malloc(nValueLen+2);
    strncpy(value, szValue, nValueLen);
    value[nValueLen] = '\0';

    return value;
}



int main(int argc, char *argv[])
{
    QGuiApplication * application = SailfishApp::application(argc, argv);

    qRegisterMetaType<QString>("QString");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QString OSDetailsString= QString("Running on : %1 Application Compiled with QT Version :  %2 Running with QT Version %3")
    .arg(QtLogView::getOsDetails().toStdString().c_str(),QT_VERSION_STR,qVersion());

    CMainWindow* m_appWindow = CMainWindow::getInstance();

    const QByteArray dir = QFileInfo(QStandardPaths::writableLocation(QStandardPaths::DataLocation)).absolutePath().toLatin1();
    m_strRootPath = std::string(dir.constData(), dir.length());
    m_strRootPath += "/rho/";


    // PreMessageLoop:
    rho_logconf_Init(m_strRootPath.c_str(), m_strRootPath.c_str(), m_logPort.c_str());

    if ( !rho_rhodesapp_canstartapp(g_strCmdLine.c_str(), " /-,") )
    {
        QMessageBox::critical(0,QString("This is hidden app and can be started only with security key."), QString("Security Token Verification Failed"));
        RAWLOGC_INFO("QTMain", "This is hidden app and can be started only with security key.");
        if (RHOCONF().getString("invalid_security_token_start_path").length() <= 0)
            return 1;
    }
    RAWLOGC_INFO("QTMain" ,OSDetailsString.toStdString().c_str());
    //Identify QT is hardcoded path . hardcoded by compiler paths will be displayed.
    for(int nCounter = 0; nCounter < QCoreApplication::libraryPaths().size(); nCounter ++)
    {
         QString path = QCoreApplication::libraryPaths().at(nCounter);
         RAWLOGC_INFO1("QTMain", "%d Paths",nCounter);
         RAWLOGC_INFO("QTMain", path.toStdString().c_str());
    }

    RAWLOGC_INFO("QTMain", "Rhodes started");

    rho::common::CRhodesApp::Create(m_strRootPath, m_strRootPath, m_strRootPath);

    RHODESAPP().setJSApplication(m_isJSApplication);

    // Create the main application window

    m_appWindow->Initialize(convertToStringW(RHODESAPP().getAppTitle()).c_str());

    RHODESAPP().startApp();

    // Navigate to the "loading..." page
    m_appWindow->navigate(L"about:blank", -1);

    if (RHOCONF().getString("test_push_client").length() > 0 )
        rho_clientregister_create(RHOCONF().getString("test_push_client").c_str());//"qt_client");






    QQuickView * view =  SailfishApp::createView();
    QObject::connect(view, &QQuickView::activeChanged, [=](){qDebug() << (view->isActive()?"Active":"Not active");});

    //QFileInfo fileInfo(QString::fromStdString(argv[0]));
    //QDirIterator dirIterator(fileInfo.absoluteDir());

    QString workingLocation("/usr/share/" + application->applicationName() + "/data/");
    QObject::connect(view->engine(), &QQmlEngine::quit, application, &QGuiApplication::quit);

    QList<QObject *> dataList;
    for (int i = 0; i < 7; i++){
        dataList.append(new CustomMenuItem("Item " + QString::number(i), application));
    }

    view->setSource(SailfishApp::pathToMainQml());
    view->showFullScreen();

    QQmlContext *ctxt = view->rootContext();
    ctxt->setContextProperty("mainMenuListModel", QVariant::fromValue(dataList));




    m_appWindow->messageLoop();
    application->exec();

    // stopping Rhodes application
    rho_ringtone_manager_stop();
    m_appWindow->DestroyUi();

    rho::common::CRhodesApp::Destroy();


    return  0;
}