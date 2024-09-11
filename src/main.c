#include "reading.h"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>


void error(const char* msg) {
    perror(msg);
    exit(0);
}

/*
 * Send a JSON HTTP POST request.
 *
 * Taken from https://stackoverflow.com/a/22135885 
 *
 */
void post_json(char* hostname, int portno, char* path, const char* json) {
    char* msg_fmt = "POST %s HTTP/1.1\nHost: localhost\nAccept: application/json\nContent-Type: application/json\nContent-Length: %d\n\n%s\r\n\r\n";
    
    struct hostent* host;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    
    char message[1024];

    sprintf(message, msg_fmt, path, strlen(json), json);
    printf("\nRequest:\n%s\n", message);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    host = gethostbyname(hostname); 
    if (host == NULL) {
        error("Error no such host");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Error connecting");
    }

    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd, message+sent, total-sent);
        if (bytes < 0) {
            error("Error writing message to socket");
        }
        if (bytes == 0) {
            break;
        }
        sent += bytes;
    } while (sent < total);

    // memset(response, 0, sizeof(response));
    // total = sizeof(response) - 1;
    // received = 0;
    // do {
    //     bytes = read(sockfd, response+received, total-received);
    //     if (bytes < 0) {
    //         error("Error writing message to socket");
    //     }
    //     if (bytes == 0) {
    //         break;
    //     }
    //     sent += bytes;
    // } while (received < total);
    //
    // if (received == total) {
    //     error("Error storing complete response from socket");
    // }

    close(sockfd);
}


int main() {
    Reading reading;
    takeReading(&reading);
    printReading(&reading);
    
    char* json = readingToJSON(&reading);

    post_json("localhost", 9001, "/", json);

    free(json);
    return 0;
}
