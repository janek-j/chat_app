#ifndef SERVER_H
#define SERVER_H

//server.h, janek-j 3.05.2025

#include <iostream>
#include <vector>
#include <winsock2.h>

class ChatServer {
  private:

    SOCKET server_socket;
    int port;
    bool running;
    std::vector<SOCKET> client_sockets;

  public:

    explicit ChatServer(int _port); //done

    void set_port(int _port); //done
    int get_port(); //done

    void set_running(bool _running); //done
    bool get_running(); //done

    void set_server_socket(SOCKET _socket); //done
    SOCKET get_server_socket(); //done

    void start_server(); //done
    void stop_server(); //done
    void accept_clients(); //TODO
    void handle_client(SOCKET cli_sock); //TODO
    void broadcast_message(const std::string &message); //TODO

    ~ChatServer(); //done
};

#endif