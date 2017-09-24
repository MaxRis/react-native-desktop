/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "reactslider.h"
#include "reactbridge.h"
#include "reacteventdispatcher.h"

#include <QDebug>

class ReactSliderPrivate {
public:
    ReactBridge* bridge = nullptr;
};

ReactSlider::ReactSlider(QObject* parent) : QObject(parent), d_ptr(new ReactSliderPrivate) {}

ReactSlider::~ReactSlider() {}

void ReactSlider::setBridge(ReactBridge* bridge) {
    Q_D(ReactSlider);
    d->bridge = bridge;
}

QString ReactSlider::moduleName() {
    return "Slider";
}

QList<ReactModuleMethod*> ReactSlider::methodsToExport() {
    return QList<ReactModuleMethod*>{};
}

QVariantMap ReactSlider::constantsToExport() {
    return QVariantMap{};
}

void ReactSlider::tempMethod() {
    emit tempCompleted();
}
