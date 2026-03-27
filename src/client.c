#include "../include/common.h"

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    FILE *file;

    if (argc != 3) {
        printf("Usage: %s <server_ip> <filename>\n", argv[0]);
        return -1;
    }

    init_sockets();

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error.\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("Invalid address / Address not supported.\n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed.\n");
        return -1;
    }

    // Open file to send - "rb" reads as binary so it is resilient against data types
    file = fopen(argv[2], "rb");
    if (file == NULL) {
        printf("Cannot open file to send.\n");
        return -1;
    }

    // Read from file and send to server
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }

    printf("File sent successfully.\n");

    fclose(file);

    // Close socket
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    cleanup_sockets();
    return 0;
}
