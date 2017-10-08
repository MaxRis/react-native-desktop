
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

function genUbuntu(args, config) {
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
