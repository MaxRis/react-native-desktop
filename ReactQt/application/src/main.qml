
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

import QtQuick 2.4
import React 0.1 as React

Rectangle {
  id: root
  width: 384
  height: 640

  React.RootView {
    anchors.fill: parent

    moduleName: "RNTesterApp"
    codeLocation: "http://localhost:8081/RNTester/js/RNTesterApp.desktop.bundle?platform=desktop&dev=true"
    serverConnectionType: ReactNativeProperties.executor
    //moduleName: "App"
    //codeLocation: "http://localhost:8081/TouchableImageApp.bundle?platform=desktop&dev=true"
  }
}
