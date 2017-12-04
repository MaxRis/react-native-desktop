
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

#include <QMetaMethod>
#include <QMetaObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QDebug>

#include "bridge.h"
#include "eventdispatcher.h"
#include "networking.h"

namespace {
QVariantMap headerListToMap(const QList<QNetworkReply::RawHeaderPair>& rawHeaderPairs) {
    QVariantMap rh;
    for (auto& hp : rawHeaderPairs) {
        rh.insert(hp.first, hp.second);
    }
    return rh;
}
} // namespace

class NetworkingPrivate {
public:
    Bridge* bridge;
    QNetworkAccessManager* nam;
    QMap<int, QNetworkReply*> activeConnections;

    void handleGetRequest(int requestId, const QNetworkRequest& request) {
        QNetworkReply* reply = nam->get(request);
        qDebug() << "Send request!";
        QObject::connect(reply, &QNetworkReply::metaDataChanged, [=]() {
            qDebug() << "QNetworkReply::metaDataChange: didReceiveNetworkResponse responseCode: "
                     << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                     << " replyUrl: " << reply->url().toString()
                     << " headersList: " << headerListToMap(reply->rawHeaderPairs());
            bridge->eventDispatcher()->sendDeviceEvent(
                "didReceiveNetworkResponse",
                QVariantList{requestId,
                             reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
                             headerListToMap(reply->rawHeaderPairs()),
                             reply->url().toString()});
        });
        QObject::connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesReceived, qint64 bytesTotal) {
            if (bytesReceived == 0) {
                return;
            }
            QByteArray receivedData = reply->read(bytesReceived);
            qDebug() << "Received data: " << receivedData;
            bridge->eventDispatcher()->sendDeviceEvent("didReceiveNetworkData", QVariantList{requestId, receivedData});
        });
        QObject::connect(reply, &QNetworkReply::finished, [=]() {
            qDebug() << "NetworkingPrivate::handleGetRequest QNetworkReply::finished requestId: " << requestId
                     << "error: " << reply->errorString();
            reply->deleteLater();
            QByteArray receivedData = reply->readAll();
            qDebug() << "Received data size: " << receivedData.size();
            bridge->eventDispatcher()->sendDeviceEvent("didReceiveNetworkData", QVariantList{requestId, receivedData});
            bridge->eventDispatcher()->sendDeviceEvent(
                "didCompleteNetworkResponse",
                QVariantList{requestId,
                             reply->error() != QNetworkReply::NoError ? QVariant(reply->errorString()) : QVariant()});
            activeConnections.remove(requestId);
        });
        activeConnections[requestId] = reply;
    }
};

void Networking::sendRequest(const QString& method,
                             const QUrl& url,
                             const QVariantMap& data,
                             const QVariantMap& headers,
                             const QString& responseType,
                             bool incrementalUpdates,
                             qlonglong timeout,
                             bool withCredentials,
                             double requestIdCallbackId) {
    // qDebug() << __PRETTY_FUNCTION__ << method << url;
    Q_D(Networking);

    qDebug() << "Networking::sendRequest " << url << headers << data << "method: " << method;

    QNetworkRequest request(url);

    for (const QString& key : headers.keys()) {
        request.setRawHeader(key.toLocal8Bit(), headers[key].toString().toLocal8Bit());
    }

    // TODO: data (post etc)

    if (method.compare("get", Qt::CaseInsensitive) == 0 || method == "unknown") {
        static int requestId = 0;
        ++requestId;
        d->bridge->invokePromiseCallback(requestIdCallbackId, QVariantList{requestId});
        d->handleGetRequest(requestId, request);
    }
}

void Networking::abortRequest(int requestId) {
    // qDebug() << __PRETTY_FUNCTION__ << request;
    Q_D(Networking);
    QNetworkReply* reply = d->activeConnections[requestId];
    if (reply == nullptr) {
        qCritical() << __PRETTY_FUNCTION__ << "Abort failed; no relevant QNetworkReply found";
        return;
    }
    reply->abort();
    reply->deleteLater();
    d->activeConnections.remove(requestId);
}

Networking::Networking(QObject* parent) : QObject(parent), d_ptr(new NetworkingPrivate) {
    Q_D(Networking);
    d->nam = nullptr;
}

Networking::~Networking() {}

void Networking::setBridge(Bridge* bridge) {
    Q_D(Networking);
    d->bridge = bridge;
    d->nam = bridge->networkAccessManager();
}

ViewManager* Networking::viewManager() {
    return nullptr;
}

QString Networking::moduleName() {
    return "RCTNetworking";
}

QList<ModuleMethod*> Networking::methodsToExport() {
    return QList<ModuleMethod*>{};
}

QVariantMap Networking::constantsToExport() {
    return QVariantMap{};
}
