
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
#include "eventdispatcher.h"
#include "websocketmodule.h"

class WebSocketModulePrivate {
public:
    Bridge* bridge;
    QMap<qlonglong, QWebSocket*> sockets;
    QMap<QWebSocket*, qlonglong> socketIds;
};

void WebSocketModule::connect(const QUrl& url,
                              const QVariantList& protocols,
                              const QVariantMap& options,
                              qlonglong socketId) {
    Q_D(WebSocketModule);

    qDebug() << "WebSocketModule::connect with args: url: " << url << " socketId: " << socketId;

    QWebSocket* socket = new QWebSocket();
    d->sockets.insert(socketId, socket);

    QObject::connect(socket, &QWebSocket::connected, [socket, socketId, this]() {
        qDebug() << "Socket connected. SocketId:" << socketId;
        if (this->d_ptr->bridge) {
            this->d_ptr->bridge->eventDispatcher()->sendDeviceEvent("websocketOpen", QVariantMap{{"id", socketId}});
        }
    });

    QObject::connect(socket, &QWebSocket::disconnected, [socket, socketId, this]() {
        this->d_ptr->sockets.remove(socketId);
        socket->deleteLater();
        qDebug() << "Socket disconnected. SocketId:" << socketId;
        if (this->d_ptr->bridge) {
            this->d_ptr->bridge->eventDispatcher()->sendDeviceEvent(
                "websocketClosed",
                QVariantMap{
                    {"id", socketId}, {"code", socket->closeCode()}, {"reason", socket->closeReason()}, {"clean", ""}});
        }
    });

    QObject::connect(socket, &QWebSocket::textMessageReceived, [socket, socketId, this](const QString& message) {
        qDebug() << QString("Text message %1 received for SocketId").arg(message).arg(socketId);
        if (this->d_ptr->bridge) {
            this->d_ptr->bridge->eventDispatcher()->sendDeviceEvent(
                "websocketMessage", QVariantMap{{"id", socketId}, {"type", "text"}, {"data", message}});
        }
    });

    QObject::connect(socket, &QWebSocket::binaryMessageReceived, [socket, socketId, this](const QByteArray& message) {
        qDebug() << QString("Binary message of size %1 received for SocketId").arg(message.size()).arg(socketId);
        if (this->d_ptr->bridge) {
            this->d_ptr->bridge->eventDispatcher()->sendDeviceEvent(
                "websocketMessage", QVariantMap{{"id", socketId}, {"type", "binary"}, {"data", message}});
        }
    });

    QObject::connect(socket,
                     static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
                     [=](QAbstractSocket::SocketError) { qDebug() << socket->errorString(); });

    socket->open(url);
}

void WebSocketModule::send(const QString& message, qlonglong socketId) {
    qDebug() << "WebSocketModule::send with args: message: " << message << " socketId: " << socketId;
    Q_D(WebSocketModule);
    if (d->sockets.contains(socketId)) {
        d->sockets[socketId]->sendTextMessage(message);
        d->sockets[socketId]->flush();
    }
}

void WebSocketModule::sendBinary(const QString& base64String, qlonglong socketId) {
    qDebug() << "WebSocketModule::sendBinary with args: base64String: " << base64String << " socketId: " << socketId;
    Q_D(WebSocketModule);
    if (d->sockets.contains(socketId)) {
        d->sockets[socketId]->sendBinaryMessage(base64String.toLocal8Bit());
        d->sockets[socketId]->flush();
    }
}

void WebSocketModule::ping(qlonglong socketId) {
    qDebug() << "WebSocketModule::ping with args: socketId: " << socketId;
    Q_D(WebSocketModule);
    if (d->sockets.contains(socketId)) {
        d->sockets[socketId]->ping();
        d->sockets[socketId]->flush();
    }
}

void WebSocketModule::close(qlonglong socketId) {
    qDebug() << "WebSocketModule::ping with args: socketId: " << socketId;
    Q_D(WebSocketModule);
    if (d->sockets.contains(socketId)) {
        d->sockets[socketId]->close();
    }
}

WebSocketModule::WebSocketModule(QObject* parent) : QObject(parent), d_ptr(new WebSocketModulePrivate) {
    Q_D(WebSocketModule);
}

WebSocketModule::~WebSocketModule() {
    Q_D(WebSocketModule);
    d->bridge = nullptr;
    foreach (QWebSocket* websocket, d->sockets.values()) {
        websocket->close();
        websocket->disconnect();
        websocket->deleteLater();
    }
}

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
