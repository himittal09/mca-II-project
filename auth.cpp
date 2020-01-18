#include <iostream>
#include <chrono>
#include <jwt-cpp/jwt.h>
#include <openssl/crypto.h>
#include <fstream>
#include "./user.cpp"

using std::string;

class AuthModule
{
private:
    bool isUserAuthenticated = false;
    string authToken;
    unsigned int authenticatedUserId = 0;
    string authSecret;

    void getSecret ()
    {
        FILE *fp = fopen("secret.txt", "r");
        if (!fp)
        {
            throw std::runtime_error("Unable to access super secret sauce for token generation!!");
        }

        string secret;
        fscanf(fp, "%s", secret);
        fclose(fp);
        this->authSecret = secret;
    }

    void authenticateUser (unsigned int userId)
    {        
        string token = jwt::create()
            .set_type("JWS")
            .set_issuer("Himanshu")
            .set_issued_at(std::chrono::system_clock::now())
	        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{3600})
	        .set_payload_claim("userId", jwt::claim(std::to_string(userId)))
            .sign(jwt::algorithm::hs256{this->authSecret});

        isUserAuthenticated = true;
        authToken = token;
        authenticatedUserId = userId;
    }

    void unauthenticateUser ()
    {
        isUserAuthenticated = false;
        authToken = "";
        authenticatedUserId = 0;
    }

    void restoreAuthState ()
    {
        bool userAuth;
        string token;
        unsigned int authenticatedUserId;

        std::fstream fileptr;
        fileptr.open("authstate.txt", std::ios::in | std::ios::out);
        if (fileptr.good())
        {
            fileptr >> authToken;
            fileptr.close();
            return;
        }
        else
        {
            unauthenticateUser();
        }
    }

    void validateToken (string token)
    {
        jwt::verifier<jwt::default_clock> verifier = jwt::verify()
	        .allow_algorithm(jwt::algorithm::hs256{ this->authSecret })
	        .with_issuer("Himanshu");
        
        jwt::decoded_jwt decoded_token = jwt::decode(token);

        verifier.verify(decoded_token);

        for (auto& e : decoded_token.get_payload_claims())
        {
            std::cout << e.first << " " << e.second.to_json() << std::endl;
        }
    }

public:
    AuthModule ()
    {
        try
        {
            this->getSecret();
            this->restoreAuthState();
        }
        catch(const std::runtime_error& e)
        {
            throw e;
        }
        
    }

    void loginUser (string email, string password)
    {
        if (isUserAuthenticated)
        {
            throw std::runtime_error("Logout current user first!!");
        }
        // check for username and password
        // generate auth token
        // store tokens in this module itself
    }

    unsigned int signupUser (string email, string password, string name)
    {
        if (isUserAuthenticated)
        {
            throw std::runtime_error("Logout current user first!!");
        }
        
        int userExists = 0;

        try
        {
            userExists = User::findOne(email);
        }
        catch(const std::runtime_error& e)
        {
            throw e;
        }
        
        if (userExists)
        {
            throw std::runtime_error("User already exists!!");
        }
        
        User *user = new User(email, password, name);
        unsigned int userId = 0;
        
        try
        {
            userId = user->save();
        }
        catch(const std::runtime_error& e)
        {
            throw e;
        }
        
        try
        {
            authenticateUser(userId);
        }
        catch(const std::runtime_error& e)
        {
            throw e;
        }

        return userId;
    }  

};