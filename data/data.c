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
    if(nbOctets == 0) buffer[0] = 0;
    return nbOctets;
}

/**
 * @brief Ecriture d'un message datagramme
 *
 * @param sock : descripteur de socket
 * @param src : adresse distante
 * @param buffer : buffer d'envoi
 * @return int : nombre d'octets écrits
 */
int ecrireDgram(int sock, struct sockaddr_in *addr, char *buffer)
{
    int nbOctets;
    CHECK(nbOctets = sendto(sock, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)addr, sizeof(*addr)), "Pb-sendto");
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
    if(nbOctets == 0) buffer[0] = 0;
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

void envoyerReqDgram(int sock, void *req, fct_Serial * reqToSerial, struct sockaddr_in *addr){
    char buffer[1024];
    (*reqToSerial)(req,buffer);
    ecrireDgram(sock, addr, buffer);
}

void lireRepStream(int sock, void *req, fct_Serial * serialToRep){
    char buffer[1024];
    lireStream(sock, buffer);
    (*serialToRep)(buffer, req);
}

void lireRepDgram(int sock, void *req, fct_Serial * serialToRep, struct sockaddr_in *src){
    char buffer[1024];
    lireDgram(sock, src, buffer);
    (*serialToRep)(buffer, req);
}