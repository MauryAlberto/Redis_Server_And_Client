#pragma once

#include <cstring>
#include <atomic>
#include <thread>

class RedisServer{

    public:
        RedisServer(int port);
        void run();
        void shutdown();
    
    private:
        int port;
        int server_socket;
        std::atomic<bool> running;

        // Setpup signal handling for graceful shutdown (ctrl + c)
        void setupSignalHandler();
};