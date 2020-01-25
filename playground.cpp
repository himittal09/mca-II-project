#include <iostream>
#include "./auth.cpp"

using namespace std;

int main ()
{
    auth::auth.authenticateUser(10);
    cout << auth::auth.authenticatedUserId << endl << auth::auth.authToken << endl << auth::auth.isUserAuthenticated << endl << auth::auth.authSecret;
    cout << endl << endl;
    // auth::auth.validateToken(auth::auth.authToken);
    return 0;
}