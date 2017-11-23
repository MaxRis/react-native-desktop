/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "externaltestmodule.h"
#include "bridge.h"
#include "eventdispatcher.h"

#include <QDebug>

namespace {
struct RegisterQMLMetaType {
    RegisterQMLMetaType() {
        qRegisterMetaType<ExternalTestModule*>();
    }
} registerMetaType;
} // namespace

class ExternalTestModulePrivate {
public:
    Bridge* bridge = nullptr;
};

ExternalTestModule::ExternalTestModule(QObject* parent) : QObject(parent), d_ptr(new ExternalTestModulePrivate) {}

ExternalTestModule::~ExternalTestModule() {}

void ExternalTestModule::setBridge(Bridge* bridge) {
    Q_D(ExternalTestModule);
    d->bridge = bridge;
}

QString ExternalTestModule::moduleName() {
    return "ExternalTestModule";
}

QList<ModuleMethod*> ExternalTestModule::methodsToExport() {
    return QList<ModuleMethod*>{};
}
QVariantMap ExternalTestModule::constantsToExport() {
    return QVariantMap{{"sayHello", "Hello, I'm ExternalTestModule!"}};
}
