/*  Establish TCP Connection to Redis (RedisClient)
    Berkley sockets to open TCP connection
    IPv4 and IPv6 `getaddrinfo`

    Implements:
        connectToServer() -> Establishing the connection
        sendCommand() -> Sends a command over the sockets
        disconnect() -> Closes the socket when finished

*/

#include "RedisClient.h"

RedisClient::RedisClient(const std::string &host, int port) 
    : host(host), port(port), sockfd(-1) {}

RedisClient::~RedisClient(){
    disconnect();
}

bool RedisClient::connectToServer(){
    struct addrinfo hints, *res = nullptr; // Clear the hints structure
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IPv4 abd IPv6
    hints.ai_socktype = SOCK_STREAM; // Use TCP
    
    std::string portStr = std::to_string(port); // Convert port number to string
    // Use getaddrinfo() to retrieve a linked list of address(es) to connect to
    int err = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res);
    if(err != 0){
        std::cerr << "getaddrinfo: " << gai_strerror(err) << "\n"; // Print error message
        return false; // Return false on failure
    }

    // Iterate through the resolved addresses
    for(auto p = res; p != nullptr; p = p->ai_next){
        // Use the socket() to open a local socket with the specified parameters
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(sockfd == -1){ // Skip if socket creation failed
            continue;
        }
        // Use the socket file descriptor to attempt a connection to the server's IP and Port
        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == 0){
            break; // Break on successful connection
        }

        close(sockfd); // Close socket if connection failed
        sockfd = -1; // Reset socket file descriptor
    }
    // Free the memory from the linked list pointed to by res
    freeaddrinfo(res);

    if(sockfd == -1){
        std::cerr << "Could not connect to " << host << ":" << port << "\n"; // Print failure message
        return false;
    }

    return true; // Return true on successful connection
}

void RedisClient::disconnect(){
    if(sockfd != -1){
        close(sockfd); // Close socket if connection failed
        sockfd = -1; // Reset socket file descriptor
    }
}

int RedisClient::getSocketFD() const{
    return sockfd;
}

bool RedisClient::sendCommand(const std::string &command){
    if(sockfd == -1){
        return false;
    }

    ssize_t sent = send(sockfd, command.c_str(), command.size(), 0);
    return (sent == (ssize_t)command.size());
}

