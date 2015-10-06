(function() {
  'use strict';

  angular
    .module('esp8266GulpAngular')
    .run(runBlock);

  /** @ngInject */
  function runBlock($log) {

    $log.debug('runBlock end');
  }

})();
