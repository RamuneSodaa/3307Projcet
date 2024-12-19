#ifndef NOTIFICATIONINTERFACE_H
#define NOTIFICATIONINTERFACE_H

#include <string>

class NotificationInterface {
public:
    static void show(const std::string& message);
};

#endif
