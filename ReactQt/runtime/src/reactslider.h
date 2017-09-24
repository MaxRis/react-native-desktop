/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTSLIDER_H
#define REACTSLIDER_H

#include "reactmoduleinterface.h"

#include <QVariantMap>

class ReactSliderPrivate;
class ReactSlider : public QObject, public ReactModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ReactModuleInterface)

    Q_DECLARE_PRIVATE(ReactSlider)

public:
    ReactSlider(QObject* parent = 0);
    ~ReactSlider();

    void setBridge(ReactBridge* bridge) override;

    QString moduleName() override;
    QList<ReactModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

    Q_INVOKABLE void tempMethod();

signals:
    void tempCompleted();

private:
    QScopedPointer<ReactSliderPrivate> d_ptr;
};

#endif // REACTSLIDER_H
