#include "data.h"

void traiterSIGCHLD(int sig)
{
    if (sig == SIGCHLD)
    {
        int status;
        wait(&status);
    }
}

/**
 * @brief Lecture d'un message datagramme
 * @param sock : descripteur de socket
 * @param src : adresse du client
 * @param buffer : buffer de réception
 * @return : nombre d'octets lus
 */
int lireDgram(int sock, struct sockaddr_in *src, char *buffer)
{
    int nbOctets;
    socklen_t srcLen = sizeof(*src);
    CHECK(nbOctets = recvfrom(sock, buffer, MAX_BUFF, 0, (struct sockaddr *)src, &srcLen), "Pb-recvfrom");

    return nbOctets;
}

/**
 * @brief Ecriture d'un message datagramme
 *
 * @param sock : descripteur de socket
 * @param src : adresse du client
 * @param buffer : buffer d'envoi
 * @return int : nombre d'octets écrits
 */
int ecrireDgram(int sock, struct sockaddr_in *src, char *buffer)
{
    int nbOctets;
    CHECK(nbOctets = sendto(sock, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)src, sizeof(src)), "Pb-sendto");
    return nbOctets;
}

/**
 * @brief Lecture d'un message stream
 *
 * @param sock : descripteur de socket
 * @param buffer : buffer de réception
 * @return int : nombre d'octets lus
 */
int lireStream(int sock, char *buffer)
{
    int nbOctets;
    CHECK(nbOctets = read(sock, buffer, MAX_BUFF), "Pb-read");
    return nbOctets;
}

/**
 * @brief Ecriture d'un message stream
 *
 * @param sock : descripteur de socket
 * @param buffer : buffer d'envoi comprenant le message à envoyé
 * @return int : nombre d'octets écrits
 */
int ecrireStream(int sock, char *buffer)
{
    int nbOctets;
    size_t nbCar = strlen(buffer) + 1;
    CHECK(nbOctets = write(sock, buffer, nbCar), "Pb-write");
    return nbOctets;
}

void envoyerReqStream(int sock, void *req, fct_Serial * reqToSerial){
    char buffer[1024];
    (*reqToSerial)(req,buffer);
    ecrireStream(sock, buffer);
}

void envoyerReqDgram(int sock, void *req, fct_Serial * reqToSerial, struct sockaddr_in *src){
    char buffer[1024];
    (*reqToSerial)(req,buffer);
    ecrireDgram(sock, src, buffer);
}