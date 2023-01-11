#include "data.h"

void traiterSIGCHLD(int sig)
{
    if(sig == SIGCHLD)
    {
        int status;
        wait(&status);
    }
}

void creerProcessusServeur(int se, int sd, struct sockaddr_in clt)
{
    int pid;
    CHECK(pid = fork(), "fork + Problème de création de processus !");
    if (pid == 0)
    {   
        close(se);
        printf("Processus fils : %d pour gerer un nouveau client\n", getpid());
        identificationServeur(sd);
        close(sd);
        exit(0);
    }
    printf("HERE");
    close(sd);
    traiterSIGCHLD(SIGCHLD);
}


/** 
 * @brief Lecture d'un message datagramme
 * @param sock : descripteur de socket
 * @param src : adresse du client
 * @param buffer : buffer de réception
 * @return : nombre d'octets lus
*/
int lireDgram(int sock, struct sockaddr_in * src, char * buffer){
    int nbOctets;
    CHECK(nbOctets = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)src, sizeof(src)), "Pb-recvfrom");
    return nbOctets;
};

/**
 * @brief Ecriture d'un message datagramme
 * 
 * @param sock : descripteur de socket
 * @param src : adresse du client
 * @param buffer : buffer d'envoi
 * @return int : nombre d'octets écrits
 */
int ecrireDgram(int sock, struct sockaddr_in * src, char * buffer){
    int nbOctets;
    CHECK(nbOctets = sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)src, sizeof(src)), "Pb-sendto");
    return nbOctets;
};

/**
 * @brief Lecture d'un message stream
 * 
 * @param sock : descripteur de socket
 * @param buffer : buffer de réception
 * @return int : nombre d'octets lus
 */
int lireStream(int sock, char * buffer){
    int nbOctets;
    CHECK(nbOctets = read(sock, buffer, sizeof(buffer)), "Pb-read");
    return nbOctets;
};

/**
 * @brief Ecriture d'un message stream
 * 
 * @param sock : descripteur de socket
 * @param buffer : buffer d'envoi
 * @return int : nombre d'octets écrits
 */
int ecrireStream(int sock, char * buffer){
    int nbOctets;
    CHECK(nbOctets = write(sock, buffer, sizeof(buffer)), "Pb-write");
    return nbOctets;
};