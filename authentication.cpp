#include <iostream>
#include "authentication.h"

using namespace std;

const string ADMIN_PASS = "admin123";
const string USER_PASS = "user456";

Role authenticate() {
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    if (username == "admin" && password == ADMIN_PASS) {
        return ADMIN;
    } else if (username == "user" && password == USER_PASS) {
        return USER;
    }

    return INVALID;
}
