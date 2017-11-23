/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef EXTERNALTESTMODULE2_H
#define EXTERNALTESTMODULE2_H

#include "moduleinterface.h"

#include <QVariantMap>

class ExternalTestModule2Private;
class ExternalTestModule2 : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)

    Q_DECLARE_PRIVATE(ExternalTestModule2)

public:
    Q_INVOKABLE ExternalTestModule2(QObject* parent = 0);
    ~ExternalTestModule2();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;
    QList<ModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

private:
    QScopedPointer<ExternalTestModule2Private> d_ptr;
};

#endif // EXTERNALTESTMODULE2_H
