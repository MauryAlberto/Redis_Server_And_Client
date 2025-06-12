#pragma once

#include <string>
#include <vector>
#include "RedisClient.h"
#include "CommandHandler.h"
#include "ResponseParser.h"

class CLI{
    public:
        CLI(const std::string &host, int port);
        void run(const std::vector<std::string>& commandArgs);
        void executeCommand(const std::vector<std::string>& args);

    private:
        std::string host;
        int port;
        RedisClient redisClient;
};