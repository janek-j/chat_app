#include "server.h"

#include <winsock2.h>

ChatServer::ChatServer(int _port) {
	set_port(_port);
    set_running(false);
    set_server_socket(INVALID_SOCKET);
}

void ChatServer::set_port(int _port) {
	port = _port;
}

int ChatServer::get_port() {
  return port;
}

void ChatServer::set_running(bool _running) {
	running = _running;
}

bool ChatServer::get_running() {
  return running;
}

void ChatServer::set_server_socket(SOCKET _socket) {
  server_socket = _socket;
}

SOCKET ChatServer::get_server_socket() {
  return server_socket;
}

void ChatServer::start_server() {

    std::cout << "Starting server on port: " << get_port() << std::endl;

    WSAData wsaData;

    int result;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData); //inicjalizacja winsock

    if(result != 0) {
        std::cerr << "Error in ChatServer::start_server()" << std::endl;
        return;
    }
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == INVALID_SOCKET) {
      std::cerr << "server_socket initialization failed in ChatServer::start_server() "  << WSAGetLastError() << std::endl;
      WSACleanup();
      return;
    }

    int server_port = get_port();

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);

    result = bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
    if(result == SOCKET_ERROR) {
       std::cerr << "bind() failed in ChatServer::start_server(). " << WSAGetLastError() << std::endl;
       closesocket(server_socket);
       WSACleanup();
       return;
    }

    result = listen(server_socket, SOMAXCONN);
    if(result == SOCKET_ERROR) {
        std::cerr << "Listen failed in ChatServer::start_server(). " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    set_running(true);
    std::cout << "Server started at port: " << server_port << std::endl;
}

void ChatServer::stop_server() {
	set_running(false);
    for (SOCKET cli_sock: client_sockets) {
      closesocket(cli_sock);
    }
    client_sockets.clear();
    closesocket(server_socket);
    WSACleanup();
    std::cout << "Server stopped" << std::endl;
}

ChatServer::~ChatServer() {
  stop_server();
}

void ChatServer::accept_clients() {
  std::cout << "Waiting for clients. " << std::endl;

   while (get_running()) {
     sockaddr_in client_addr;
     int client_len = sizeof(client_addr);
     SOCKET client_socket = accept(server_socket, (SOCKADDR*)&client_addr, &client_len);
     if (client_socket == INVALID_SOCKET) {
       std::cerr << "Accept in ChatServer::accept_clients(). " << WSAGetLastError() << std::endl;
       continue;
     }
     std::cout << "Client connected." << std::endl;
     client_sockets.push_back(client_socket);

   }

}

void ChatServer::handle_client(SOCKET cli_sock) {
    char buffer[1024];

    while (get_running()) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(cli_sock, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            std::cout << "Client disconnected." << std::endl;
            closesocket(cli_sock);
            break;
        }

        std::string msg(buffer);
        std::cout << "Received: " << msg << std::endl;

        broadcast_message(msg);
    }
}

void ChatServer::broadcast_message(const std::string &message) {
    for (SOCKET sock : client_sockets) {
        send(sock, message.c_str(), message.length(), 0);
    }
}
