
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

'use strict';

const fs = require('fs');
var generate = require('../generate/generate');
const { exec } = require('child_process');

function applyUbuntuPlatformPatch() {
  exec('pwd && patch --verbose -d ./node_modules/metro-bundler/src -i ../../react-native/add-ubuntu-platform.patch', (err, stdout, stderr) => {
    if (err) {
      console.log(`Std output: ${stdout}`);
      console.error(`exec error: ${err}`);
      return;
    }

    console.log(`Std output: ${stdout}`);
  });
}

function genUbuntu(args, config) {
  applyUbuntuPlatformPatch();
  return generate([
    '--platform', 'ubuntu',
    '--project-path', process.cwd(),
    '--project-name', JSON.parse(
      fs.readFileSync('package.json', 'utf8')
    ).name
  ], config);
}

module.exports = {
  name: 'ubuntu',
  description: 'generates an Ubuntu project for your app',
  func: genUbuntu
};
