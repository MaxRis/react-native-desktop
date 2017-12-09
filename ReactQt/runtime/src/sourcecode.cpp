
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

#include <memory>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

#include "sourcecode.h"

class SourceCodePrivate {
public:
    Bridge* bridge = nullptr;
    QUrl scriptUrl;
    QByteArray sourceCode;
    int retryCount = 4;
    int retryAttempts = 0;
    int retryTimeout = 250;
};

void SourceCode::getScriptText(const ModuleInterface::ListArgumentBlock& resolve,
                               const ModuleInterface::ListArgumentBlock& reject) {
    Q_D(SourceCode);
    if (!d->sourceCode.isNull())
        resolve(d->bridge, QVariantList{QVariantMap{{"text", d->sourceCode}, {"url", d->scriptUrl.toString()}}});
    else
        reject(d->bridge, QVariantList{QVariantMap{{"text", "Source code is not available"}}});
}

SourceCode::SourceCode(QObject* parent) : QObject(parent), d_ptr(new SourceCodePrivate) {}

SourceCode::~SourceCode() {}

void SourceCode::setBridge(Bridge* bridge) {
    Q_D(SourceCode);
    d->bridge = bridge;
}

ViewManager* SourceCode::viewManager() {
    return nullptr;
}

QString SourceCode::moduleName() {
    return "RCTSourceCode";
}

QList<ModuleMethod*> SourceCode::methodsToExport() {
    return QList<ModuleMethod*>{};
}

QVariantMap SourceCode::constantsToExport() {
    return QVariantMap{};
}

QUrl SourceCode::scriptUrl() const {
    return d_func()->scriptUrl;
}

void SourceCode::setScriptUrl(const QUrl& scriptUrl) {
    Q_D(SourceCode);
    if (d->scriptUrl == scriptUrl)
        return;
    d->scriptUrl = scriptUrl;
}

QByteArray SourceCode::sourceCode() const {
    return d_func()->sourceCode;
}

int SourceCode::retryCount() const {
    return d_func()->retryCount;
}

void SourceCode::setRetryCount(int retryCount) {
    Q_D(SourceCode);
    if (d->retryCount == retryCount)
        return;
    d->retryCount = retryCount;
    Q_EMIT retryCountChanged();
}

void SourceCode::loadSource(QNetworkAccessManager* nam) {
    Q_D(SourceCode);

    QNetworkRequest request(d->scriptUrl);
    QNetworkReply* reply = nam->get(request);
    QObject::connect(reply, &QNetworkReply::finished, [=]() {

        if (reply->error() != QNetworkReply::NoError) {
            reply->deleteLater();
            if (d->retryAttempts < d->retryCount) {
                d->retryAttempts++;
                d->retryTimeout *= 2;
                QTimer::singleShot(d->retryTimeout, [=] { loadSource(nam); });
            } else {
                qCritical() << __PRETTY_FUNCTION__ << ": Error while loading source" << reply->errorString();
                Q_EMIT loadFailed();
            }
            return;
        }
        d->sourceCode = reply->readAll();
        d->retryAttempts = 0;
        d->retryTimeout = 250;
        reply->deleteLater();
        Q_EMIT sourceCodeChanged();
    });
}
