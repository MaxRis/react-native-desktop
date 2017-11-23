/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "externaltestmodule2.h"
#include "bridge.h"
#include "eventdispatcher.h"

#include <QDebug>

namespace {
struct RegisterQMLMetaType {
    RegisterQMLMetaType() {
        qRegisterMetaType<ExternalTestModule2*>();
    }
} registerMetaType;
} // namespace

class ExternalTestModule2Private {
public:
    Bridge* bridge = nullptr;
};

ExternalTestModule2::ExternalTestModule2(QObject* parent) : QObject(parent), d_ptr(new ExternalTestModule2Private) {}

ExternalTestModule2::~ExternalTestModule2() {}

void ExternalTestModule2::setBridge(Bridge* bridge) {
    Q_D(ExternalTestModule2);
    d->bridge = bridge;
}

QString ExternalTestModule2::moduleName() {
    return "ExternalTestModule2";
}

QList<ModuleMethod*> ExternalTestModule2::methodsToExport() {
    return QList<ModuleMethod*>{};
}
QVariantMap ExternalTestModule2::constantsToExport() {
    return QVariantMap{{"sayHello", "Hello, I'm ExternalTestModule2!"}};
}
