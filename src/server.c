#include "../include/common.h"

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    FILE *file;

    init_sockets();

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation failed.\n");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Bind failed.\n");
        return 1;
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        printf("Listen failed.\n");
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        printf("Accept failed.\n");
        return 1;
    }

    printf("Connection accepted.\n");

    // Open file to save received data - "wb" writes as binary so it handles any file type in future
    file = fopen("received_file.txt", "wb");
    if (file == NULL) {
        printf("Cannot open file for writing.\n");
        return 1;
    }

    // Read data from the client and write it to the file
    int bytes_read;
    while ((bytes_read = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_read, file);
    }

    printf("File received successfully.\n");

    fclose(file);

    // Close sockets
#ifdef _WIN32
    closesocket(new_socket);
    closesocket(server_fd);
#else
    close(new_socket);
    close(server_fd);
#endif

    cleanup_sockets();
    return 0;
}
