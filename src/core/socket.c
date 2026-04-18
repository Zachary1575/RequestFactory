#include <unistd.h> // close
#include <stdlib.h> // addrinfo
#include <sys/types.h> // addrinfo
#include <sys/socket.h> // addrinfo, bind, socket, listen, accept
#include <netdb.h> // addrinfo (VERY IMPORTANT TO IMPORT)
#include <stdio.h>
#include <ctype.h>
#include <string.h> // memset

enum {
    TRUE = 1,
    FALSE = 0,
    true = 1,
    false = 0,
};

void scramble_string(char *s) {
    if (s == NULL) {
        return;
    }

    size_t len = strlen(s);
    if (len < 2) {
        return;
    }

    for (size_t i = len - 1; i > 0; i--) {
        size_t j = (size_t)(rand() % (i + 1));

        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

int main(void) {
    // Connect to given server via TCP.
    // Then we need to send request.
    // Wait for response given a timeout.
    // If message is recieved, it needs to be processed.
    // Need some kind of server that simulates the processing.

    // Setup some general information about our client socket.
    unsigned short portno = 9000;
    char* hostname = "localhost";

    char port_str[6]; // We have to convert the port into a string so we can use it in getaddr

    // 1. Create a template for socket bind information.
    int getaddrinfo_status;
    struct addrinfo hints;
    struct addrinfo *client_info; // Empty pointer that will store the addrinfo from the getaddrinfo call.

    memset(&hints, 0, sizeof hints); // sanatize the struct.
    hints.ai_family = AF_UNSPEC; // Support for both IPv4 and IPv6.
    hints.ai_socktype = SOCK_STREAM; // TCP Socket Stream

    // Since this is a client, we let populate node. We CANNOT have both be NULL.
    sprintf(port_str, "%d", portno); // Conversion of portno to port string.
    getaddrinfo_status = getaddrinfo(hostname, port_str, &hints, &client_info); // 0 on success.
    if (getaddrinfo_status != 0) {
        perror("getaddrinfo() failed!");
        return(EXIT_FAILURE);
    }

    // 2. Create socket 
    int tcp_protocol_number = getprotobyname("TCP")->p_proto;
    int socket_fd = socket(client_info->ai_family, client_info->ai_socktype, tcp_protocol_number);
    if (socket_fd == -1) {
        perror("socket() failed!");
        return (EXIT_FAILURE);
    }

    // 3. Since we are the client, we just need to connect.
    int connect_status = connect(socket_fd, client_info->ai_addr, client_info->ai_addrlen);
    if (connect_status == -1) {
        perror("connect() failed!");
        return (EXIT_FAILURE);
    }

    char message[] = "Here is a Sample Message. We hope we can shuffle these characters so we can see the cache fluctuate.";
    int default_flags_for_send = 0;

    while(true) {
        // 4. Here we send a message to the target server. This returns the number of bytes sent.
        scramble_string(message);
        int num_bytes_sent = send(socket_fd, message, strlen(message), default_flags_for_send);
        if (num_bytes_sent == -1){
            perror("send() failed");
            exit(EXIT_FAILURE);
        }

        // 5. Received data from the server.
        char client_buffer[strlen(message)]; // Server Buffer
        memset(client_buffer, 0, sizeof client_buffer);

        int default_flags_recv = 0;
        int num_bytes_recv = recv(socket_fd, &client_buffer, sizeof client_buffer, default_flags_recv);
        if (num_bytes_recv == -1){
            printf("recv() was not successful and threw an error status code of %d!\n", num_bytes_recv);
            exit(EXIT_FAILURE);
        }

        if (num_bytes_recv > 0) {
            for (int i = 0; i < (int) sizeof client_buffer; i++) {
                printf("%c", client_buffer[i]);
            }
            printf("\n");
        }
    }

    // (7) Cleanup and deallocation.
    close(socket_fd); // Close the client socket to prevent anymore read/writes.
    freeaddrinfo(client_info); // addrinfo uses malloc under the hood so we need to free it.

    return 0;
}











