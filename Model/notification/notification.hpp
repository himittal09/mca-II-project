#include <vector>
#include <iostream>

class NotificationService;

#ifndef __NOTIFICATIONH__
#define __NOTIFICATIONH__

class NotificationService
{
public:
    unsigned int userId;
    std::string notification;
    
    NotificationService () noexcept;
    NotificationService (std::string notification, unsigned int userId) noexcept;

    static void pushNotification (std::string notification, unsigned int userId) noexcept(false);
    static std::vector<std::string> getAllNotifications (unsigned int userId) noexcept(false);
};

#endif