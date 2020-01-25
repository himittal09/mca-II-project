#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _CHRONO_
    #define _CHRONO_
    #include <chrono>
#endif

#ifndef _FSTREAM_
    #define _FSTREAM_
    #include <fstream>
#endif

#ifndef _JWT_
    #define _JWT_
    #include <jwt-cpp\jwt.h>
#endif

#ifndef _CSTRING_
    #define _CSTRING_
    #include <cstring>
#endif

#ifndef __USER__
    #define __USER__
    #include "./user.cpp"
#endif

class AuthModule
{
public:
    bool isUserAuthenticated = false;
    std::string authToken;
    unsigned int authenticatedUserId = 0;
    std::string authSecret;

    void getSecret ()
    {
        std::ifstream fileptr;
        fileptr.open("secret.txt", std::ios::in);
        if (!fileptr.good())
        {
            throw std::runtime_error("Unable to access super secret sauce for token generation!!");
        }
        std::string secret;    
        fileptr >> secret;
        this->authSecret = secret;
        fileptr.close();
    }

    void authenticateUser (unsigned int userId)
    {

        std::string token = jwt::create()
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
        std::string token;
        unsigned int authenticatedUserId;

        std::fstream fileptr;
        fileptr.open("authstate.txt", std::ios::in | std::ios::out);
        if (fileptr.good())
        {
            fileptr >> authToken;
            // validateToken(authToken);
        }
        fileptr.close();
    }

    void validateToken (std::string token)
    {
        // jwt::verifier<jwt::default_clock> verifier = jwt::verify()
	    //     .allow_algorithm(jwt::algorithm::hs256{ this->authSecret })
	    //     .with_issuer("Himanshu");
        
        // jwt::decoded_jwt decoded_token = jwt::decode(token);

        // verifier.verify(decoded_token);

        // for (auto& e : decoded_token.get_payload_claims())
        // {
        //     std::cout << e.first << " " << e.second.to_json() << std::endl;
        // }
    }

public:
    AuthModule ()
    {
        try
        {
            this->getSecret();
            // this->restoreAuthState();
        }
        catch(const std::runtime_error& e)
        {
            throw e;
        }
        
    }

    void loginUser (std::string email, std::string password)
    {
        if (isUserAuthenticated)
        {
            throw std::runtime_error("Logout current user first!!");
        }
        // check for username and password
        // generate auth token
        // store tokens in this module itself
    }

    unsigned int signupUser (std::string email, std::string password, std::string name)
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


namespace auth
{
    AuthModule auth;
}