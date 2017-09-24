/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "reactpropertytestcase.h"
#include "reactview.h"
#include <QDebug>
#include <QTest>
#include <QtQuick/QQuickView>

class TestSliderProps : public ReactPropertyTestCase {
    Q_OBJECT

private slots:

    virtual void initTestCase() override;

protected:
    virtual QQuickItem* control() const override;
    virtual QVariantMap propValues() const override;
};

QQuickItem* TestSliderProps::control() const {
    QList<QQuickItem*> reactViewChilds = rootView()->childItems();
    Q_ASSERT(reactViewChilds.count() == 1);

    QQuickItem* view = reactViewChilds[0];
    QList<QQuickItem*> viewChilds = view->childItems();
    Q_ASSERT(viewChilds.count() == 1);

    QQuickItem* slider = viewChilds[0];
    Q_ASSERT(slider);

    return slider;
}

void TestSliderProps::initTestCase() {
    ReactPropertyTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestSliderProps.qml"));
    waitAndVerifyJsAppStarted();
}

QVariantMap TestSliderProps::propValues() const {
    return {{"p_value", 0.5},
            {"p_step", 0.1},
            {"p_minimumValue", 0},
            {"p_maximumValue", 1.0},
            {"p_disabled", false},
            {"p_testID", "testSlider"}};
}

QTEST_MAIN(TestSliderProps)
#include "test-slider-props.moc"
