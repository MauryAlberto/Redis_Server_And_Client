#include "RedisServer.h"
#include "RedisDatabase.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]){
    int port = 6379; // default port number
    if(argc >= 2){
        port = std::stoi(argv[1]); // changing the default port number
    }

    if(RedisDatabase::getInstance().load("dump.my_rdb")){
        std::cout << "Database loaded from dump.my_rdb\n";
    }else{
        std::cout << "No dump found or load failed; starting with an empty database.\n";
    }

    RedisServer server(port);

    // Background persistance: dump every 300 seconds. (5 * 60 save database)
    std::thread persistanceThread([](){
        while(true){
            std::this_thread::sleep_for(std::chrono::seconds(300));
            if(!RedisDatabase::getInstance().dump("dump.my_rdb")){
                std::cerr << "Error Dumping Database\n";
            }else{
                std::cout << "Database Dumped to dump.my_rdb\n";
            }
        }
    });

    persistanceThread.detach();

    server.run();

    return 0;
}