// Copyright IBM Corp. 2015. All Rights Reserved.
// Node module: loopback-getting-started-intermediate
// This file is licensed under the MIT License.
// License text available at https://opensource.org/licenses/MIT
var this_year = new Date().getFullYear();
angular
  .module('app')
  .factory('AuthService_operator', ['Operator', '$q', '$rootScope', '$state', function (
    Operator, $q, $rootScope, $state) {
    function login(email, password) {
      return Operator
        .login({ email: email, password: password })
        .$promise
        .then(function (response) {
          var birthday = new Date(response.user.birth);
          var birth_year = birthday.getFullYear();
          $rootScope.currentOperator = {
            id: response.user.id,
            tokenId: response.id,
            email: response.user.email,
            name: response.user.firstName +" "+ response.user.lastName,
            birth: response.user.birth,
            gender: response.user.gender,
            age: this_year - birth_year + 1,
            address: response.user.address
          };
        });
    }
    // function Naver(email) {
    //   return User
    //     .login({email: email})
    //     .$promise
    //     .then(function(response) {
    //       $rootScope.currentOperator = {
    //         id: response.user.profile.username,
    //         tokenId: response.user.profile.tokenId,
    //         email: response.user.profile.email
    //       };
    //     });
    // }

    function logout() {
      return Operator
        .logout()
        .$promise
        .then(function () {
          $rootScope.currentOperator = null;
        });
    }

    function register(email, password, firstName, lastName, birth, gender, address) {
      return Operator
        .create({
          email: email,
          password: password,
          firstName: firstName,
          lastName: lastName,
          birth: birth,
          gender: gender,
          address:address
        })
        .$promise;
    }

    function refresh(accessTokenId) {
      return Operator
        .getCurrent(function (userResource) {
          $rootScope.currentOperator = {
            id: userResource.id,
            tokenId: accessTokenId,
            email: userResource.email,
            name: userResource.firstName + " " + userResource.lastName,
            address: userResource.address
          };
        });
    }
    return {
      login: login,
      logout: logout,
      register: register,
      refresh: refresh
    };
  }]);
