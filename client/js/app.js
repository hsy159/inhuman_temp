// Copyright IBM Corp. 2015. All Rights Reserved.
// Node module: loopback-getting-started-intermediate
// This file is licensed under the MIT License.
// License text available at https://opensource.org/licenses/MIT



angular
  .module('app', [
    'ui.router',
    'lbServices'
  ])
  .config(['$stateProvider', '$urlRouterProvider', function ($stateProvider,
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
      .state('myreport', {
        url: '/myreport',
        templateUrl: 'views/report_my.html',
        controller: 'MyReportController'
      })
      // .state('report', {
      //   url: '/report',
      //   templateUrl: 'views/report.html',
      //   controller:'ReportController'
      // })
      // .state('addreport', {
      //   url: '/addreport',
      //   templateUrl: 'views/report_add.html',
      //   controller:'AddReportController'
      // })
      // .state('file', {
      //   url: '/file',
      //   templateUrl: 'views/file.html',
      //   controller:'FileController'
      // })
      .state('login', {
        url: '/login',
        templateUrl: 'views/login.html',
        controller: 'AuthLoginController_client'
      })
      .state('login-operator', {
        url: '/login-operator',
        templateUrl: 'views/login-operator.html',
        controller: 'AuthLoginController_operator'
      })
      .state('logout', {
        url: '/logout',
        controller: 'AuthLogoutController_client'
      })
      .state('sign-up-agree-client', {
        url: '/sign-up-agree-client',
        templateUrl: 'views/sign-up-agree-client.html'
      })
      .state('sign-up-agree-operator', {
        url: '/sign-up-agree-operator',
        templateUrl: 'views/sign-up-agree-operator.html'
      })
      .state('sign-up-client', {
        url: '/sign-up-client',
        templateUrl: 'views/sign-up-form-client.html',
        controller: 'SignUpController_client'
      })
      .state('sign-up-operator', {
        url: '/sign-up-operator',
        templateUrl: 'views/sign-up-form-operator.html',
        controller: 'SignUpController_operator'
      })
      .state('sign-up-success', {
        url: '/sign-up-success',
        templateUrl: 'views/sign-up-success.html'
      })
      .state('account', {
        url: '/account',
        templateUrl: 'views/account.html'
      })
      .state('3D_View', {
        url: '/3D_View',
        templateUrl: 'views/3D_View.html',
        controller: '3DController'
      }) 
      .state('management', {
        url: '/management',
        templateUrl: 'views/management.html',
        controller: 'ManagementController'
      })
      .state('client-history-compare', {
        url: '/client-history-compare',
        templateUrl: 'views/client-history-compare.html',
        controller: 'MyReportController'
      })
      .state('addon', {
        url: '/addon',
        templateUrl: 'views/addon.html',
        controller: 'AddOnController'
      })       
      .state('bodysize', {
        url: '/bodysize',
        templateUrl: 'views/bodysize.html',
        controller: 'BodySizeController'
      }) 
    $urlRouterProvider.otherwise('home');
  }])
  .run(['$rootScope', '$state', 'LoopBackAuth', 'AuthService_client', function ($rootScope, $state, LoopBackAuth, AuthService_client) {
    $rootScope.$on('$stateChangeStart', function (event, toState, toParams) {
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
    if (LoopBackAuth.accessTokenId && !$rootScope.currentClient) {
      AuthService_client.refresh(LoopBackAuth.accessTokenId);
    }
  }]);
