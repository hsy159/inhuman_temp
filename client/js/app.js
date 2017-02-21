// Copyright IBM Corp. 2015. All Rights Reserved.
// Node module: loopback-getting-started-intermediate
// This file is licensed under the MIT License.
// License text available at https://opensource.org/licenses/MIT



angular
  .module('app', [
    'ui.router',
    'lbServices'
  ])
  .config(['$stateProvider', '$urlRouterProvider', function($stateProvider,
      $urlRouterProvider) {
    $stateProvider
      .state('forbidden', {
        url: '/forbidden',
        templateUrl: 'views/forbidden.html'
      })
      .state('home', {
        url: '/home',
        templateUrl: 'views/home.html'
      })
      .state('report', {
        url: '/report',
        templateUrl: 'views/report.html',
        controller:'ReportController'
      })
      .state('3D_View', {
        url: '/3D_View',
        templateUrl: 'views/3D_View.html'
      })
      .state('myreport', {
        url: '/myreport',
        templateUrl: 'views/report_my.html',
        controller:'MyReportController'
      })
      .state('addreport', {
        url: '/addreport',
        templateUrl: 'views/report_add.html',
        controller:'AddReportController'
      })
      .state('addreportfile', {
        url: '/addreportfile',
        templateUrl: 'views/report_fileadd.html',
        //controller:'AddReportFileController'
      })
      .state('addfile', {
        url: '/addfile',
        templateUrl: 'views/file_upload.html'
      })
      .state('file', {
        url: '/file',
        templateUrl: 'views/file.html',
        controller:'FileController'
      })
      .state('login', {
        url: '/login',
        templateUrl: 'views/login.html',
        controller: 'AuthLoginController'
      })
      .state('logout', {
        url: '/logout',
        controller: 'AuthLogoutController'
      })
      .state('sign-up', {
        url: '/sign-up',
        templateUrl: 'views/sign-up-form.html',
        controller: 'SignUpController'
      })
      .state('sign-up-success', {
        url: '/sign-up-success',
        templateUrl: 'views/sign-up-success.html'
      })
      .state('account', {
        url: '/account',
        templateUrl: 'views/account.html'
      })
    $urlRouterProvider.otherwise('account');
  }])
  .run(['$rootScope', '$state', 'LoopBackAuth', 'AuthService', function($rootScope, $state, LoopBackAuth, AuthService) {
    $rootScope.$on('$stateChangeStart', function(event, toState, toParams) {
      // redirect to login page if not logged in
      if (toState.authenticate && !LoopBackAuth.accessTokenId) {
        event.preventDefault(); //prevent current page from loading

        // Maintain returnTo state in $rootScope that is used
        // by authService.login to redirect to after successful login.
        // http://www.jonahnisenson.com/angular-js-ui-router-redirect-after-login-to-requested-url/
        $rootScope.returnTo = {
          state: toState,
          params: toParams
        };

        $state.go('forbidden');
      }
    });

    // Get data from localstorage after pagerefresh
    // and load user data into rootscope.
    if (LoopBackAuth.accessTokenId && !$rootScope.currentUser) {
      AuthService.refresh(LoopBackAuth.accessTokenId);
    }
  }]);
