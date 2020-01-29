#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _VECTOR_
    #define _VECTOR_
    #include <vector>
#endif

class NotificationService
{
public:
    unsigned int userId;
    std::string notification;
    
    NotificationService ();
    NotificationService (std::string notification, unsigned int userId);

    static void pushNotification (std::string notification, unsigned int userId) noexcept(false);
    static std::vector<std::string> getAllNotifications (unsigned int userId) noexcept(false);

    friend std::ifstream& operator >> (std::ifstream& stream, NotificationService& obj);
    friend std::ofstream& operator << (std::ofstream& stream, NotificationService& obj);
};