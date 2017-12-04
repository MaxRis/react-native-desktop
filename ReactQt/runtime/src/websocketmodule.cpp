
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QWebSocket>

#include <QDebug>

#include "bridge.h"
#include "websocketmodule.h"

class WebSocketModulePrivate {
public:
    Bridge* bridge;
    QMap<qlonglong, QWebSocket*> sockets;
};

void WebSocketModule::connect(const QUrl &url, const QVariantList &protocols, const QVariantMap &options, qlonglong socketId)
{
    Q_D(WebSocketModule);

    QWebSocket* socket = new QWebSocket();
    d->sockets.insert(socketId, socket);


    QObject::connect(socket, &QWebSocket::connected, [=]() {
        qDebug() << "Socket connected!";
    });

    QObject::connect(socket,
            static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
            [=](QAbstractSocket::SocketError) { qDebug() << socket->errorString(); });


    socket->open(url);

}

void WebSocketModule::send(const QString &message, qlonglong socketId)
{

}

void WebSocketModule::sendBinary(const QString &base64String, qlonglong socketId)
{

}

void WebSocketModule::ping(qlonglong socketId)
{

}

void WebSocketModule::close(qlonglong socketId)
{

}

WebSocketModule::WebSocketModule(QObject* parent) : QObject(parent), d_ptr(new WebSocketModulePrivate) {
    Q_D(WebSocketModule);
}

WebSocketModule::~WebSocketModule() {}

void WebSocketModule::setBridge(Bridge* bridge) {
    Q_D(WebSocketModule);
    d->bridge = bridge;
}

ViewManager* WebSocketModule::viewManager() {
    return nullptr;
}

QString WebSocketModule::moduleName() {
    return "WebSocketModule";
}

QList<ModuleMethod*> WebSocketModule::methodsToExport() {
    return QList<ModuleMethod*>{};
}

QVariantMap WebSocketModule::constantsToExport() {
    return QVariantMap{};
}
