#include "async_tcp_socket.h"

#include "../async_network_template/async_socket.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>
#include <stdio.h>

void tcp_connect_task_handler(void* connect_task_info);

async_socket* async_tcp_connect(async_socket* connecting_tcp_socket, char* ip_address, int port, void(*connection_handler)(async_socket*, void*), void* arg){
    async_connect_info curr_connect_info;
    strncpy(curr_connect_info.ip_address, ip_address, MAX_IP_STR_LEN);
    curr_connect_info.port = port;
    
    return async_connect(connecting_tcp_socket, &curr_connect_info, tcp_connect_task_handler, connection_handler, arg);
}

async_socket* async_tcp_create_connection(char* ip_address, int port, void(*connection_handler)(async_socket*, void*), void* arg){
    async_socket* new_socket = async_socket_create();

    return async_tcp_connect(
        new_socket,
        ip_address,
        port,
        connection_handler,
        arg
    );
}

void tcp_connect_task_handler(void* connect_task_info){
    async_connect_info* connect_info = (async_connect_info*)connect_task_info;
    connect_info->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    //TODO: zero out bytes for this struct before assigning values?
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(connect_info->port);
    server_address.sin_addr.s_addr = inet_addr(connect_info->ip_address);

    int connection_status = connect(connect_info->socket_fd, (struct sockaddr*)(&server_address), sizeof(server_address));
    if(connection_status == -1){
        perror("connect()");
        connect_info->socket_fd = -1;
    }
}