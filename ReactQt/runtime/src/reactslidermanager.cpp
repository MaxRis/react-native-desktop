
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include <QDebug>

#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactevents.h"
#include "reactflexlayout.h"
#include "reactpropertyhandler.h"
#include "reactslidermanager.h"

namespace {
const QString EVENT_ON_VALUE_CHANGED = "onValueChange";
const QString EVENT_ON_SLIDING_COMPLETE = "onSlidingComplete";
}

class ReactSliderManagerPrivate {};

ReactSliderManager::ReactSliderManager(QObject* parent)
    : ReactViewManager(parent), d_ptr(new ReactSliderManagerPrivate) {}

ReactSliderManager::~ReactSliderManager() {}

ReactViewManager* ReactSliderManager::viewManager() {
    return this;
}

QString ReactSliderManager::moduleName() {
    return "RCTSliderManager";
}

QStringList ReactSliderManager::customDirectEventTypes() {
    return QStringList{
        normalizeInputEventName(EVENT_ON_VALUE_CHANGED), normalizeInputEventName(EVENT_ON_SLIDING_COMPLETE),
    };
}

void ReactSliderManager::sendSliderValueChangedToJs(QQuickItem* button) {
    int reactTag = ReactAttachedProperties::get(button)->tag();
    bridge()->enqueueJSCall(
        "RCTEventEmitter", "receiveEvent", QVariantList{reactTag, normalizeInputEventName(EVENT_ON_VALUE_CHANGED), {}});
}

void ReactSliderManager::sendSlidingCompleteToJs(QQuickItem* button) {
    int reactTag = ReactAttachedProperties::get(button)->tag();
    bridge()->enqueueJSCall("RCTEventEmitter",
                            "receiveEvent",
                            QVariantList{reactTag, normalizeInputEventName(EVENT_ON_SLIDING_COMPLETE), {}});
}

QString ReactSliderManager::qmlComponentFile() const {
    return ":/qml/ReactSlider.qml";
}

void ReactSliderManager::configureView(QQuickItem* view) const {
    ReactViewManager::configureView(view);
    view->setProperty("sliderManager", QVariant::fromValue((QObject*)this));
    if (shouldLayout()) {
        ReactFlexLayout::get(view)->setQmlImplicitWidth(true);
        ReactFlexLayout::get(view)->setQmlImplicitHeight(true);
    }
}

#include "reactslidermanager.moc"
