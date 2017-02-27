// Copyright IBM Corp. 2015. All Rights Reserved.
// Node module: loopback-getting-started-intermediate
// This file is licensed under the MIT License.
// License text available at https://opensource.org/licenses/MIT
angular
  .module('app')
  .controller('AuthLoginController_client', ['$scope', 'AuthService_client', '$state',
      function($scope, AuthService_client, $state) {
    $scope.user = {
      email: 'cc@bar.com',
      password: 'foobar'
    };

    $scope.login = function() {
      AuthService_client.login($scope.user.email, $scope.user.password)
        .then(function() {

          // return to saved returnTo state before redirection to login
          if ($scope.returnTo && $scope.returnTo.state) {
            $state.go(
              $scope.returnTo.state.name,
              $scope.returnTo.params
            );
            // maintain the inherited rootscope variable returnTo
            // but make the returnTo state of it null,
            // so it can be used again after a new login.
            $scope.returnTo.state  = null;
            $scope.returnTo.params = null;
            return;
          }
          // or go to the default state after login
          $state.go('account');
        });
    };
    // $scope.Naver = function() {
    //   AuthService.Naver($scope.user.email)
    //     .then(function() {

    //       // return to saved returnTo state before redirection to login
    //       if ($scope.returnTo && $scope.returnTo.state) {
    //         $state.go(
    //           $scope.returnTo.state.name,
    //           $scope.returnTo.params
    //         );
    //         // maintain the inherited rootscope variable returnTo
    //         // but make the returnTo state of it null,
    //         // so it can be used again after a new login.
    //         $scope.returnTo.state  = null;
    //         $scope.returnTo.params = null;
    //         return;
    //       }
    //       // or go to the default state after login
    //       $state.go('account');
    //     });
    // };
  }])
  .controller('AuthLogoutController_client', ['$scope', 'AuthService_client', '$state',
      function($scope, AuthService_client, $state) {
    AuthService_client.logout()
      .then(function() {
        $state.go('login');
      });
  }])
  .controller('SignUpController_client', ['$scope', 'AuthService_client', '$state',
      function($scope, AuthService_client, $state) {
    $scope.user = {
      email: 'foo@bar.com',
      password: 'foobar',
      firstName: 'foo',
      lastName: 'bar',
      address: 'address'
    };

    $scope.register = function() {
       AuthService_client.register($scope.user.email, $scope.user.password, $scope.user.firstName, $scope.user.lastName, $scope.user.birth, $scope.user.gender, $scope.user.address)
        .then(function() {
          $state.transitionTo('sign-up-success');
        });
    };
  }]
  
  
  
  
  
  );
