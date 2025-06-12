#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>

class RedisCommandHandler{

    public:
        RedisCommandHandler();
        // Process a command from a client and return RESP-formatted reponse
        std::string processCommand(const std::string& comandline);
    private:
    
};