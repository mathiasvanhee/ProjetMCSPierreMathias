#include "data.h"

void traiterSIGCHLD(int sig)
{
    if(sig == SIGCHLD)
    {
        int status;
        wait(&status);
    }
}

void creerProcessusServeur(int se, int sd, struct sockaddr_in * clt)
{
    int pid;
    CHECK(pid = fork(), "fork + Problème de création de processus !");
    if (pid == 0)
    {   
        close(se);
        printf("Processus fils : %d pour gerer un nouveau client\n", getpid());
        dialogueAvecClient(sd, clt);
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
    socklen_t srcLen = sizeof(*src);
    CHECK(nbOctets = recvfrom(sock, buffer, strlen(buffer), 0, (struct sockaddr *)src, &srcLen), "Pb-recvfrom");
    
    
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
int ecrireDgram(int sock, struct sockaddr_in * src, char * buffer){
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
int lireStream(int sock, char * buffer){
    int nbOctets;
    CHECK(nbOctets = read(sock, buffer, strlen(buffer)), "Pb-read");
    return nbOctets;
}

/**
 * @brief Ecriture d'un message stream
 * 
 * @param sock : descripteur de socket
 * @param buffer : buffer d'envoi comprenant le message à envoyé
 * @return int : nombre d'octets écrits
 */
int ecrireStream(int sock, char * buffer){
    int nbOctets;
    printf("Envoi d'une chaine. Longueur de la chaine : %ld\n", strlen(buffer));
    CHECK(nbOctets = write(sock, buffer, strlen(buffer) + 1), "Pb-write");
    return nbOctets;
}

void dialogueAvecClient(int sd, struct sockaddr_in * clt){
    char buffer[MAX_BUFF];
    int nbOctets;
    PAUSE("PAUSE");
    lireStream(sd, buffer);
    printf("Message reçu de [%s:%d] : \"%s\"\n", inet_ntoa(clt->sin_addr), ntohs(clt->sin_port), buffer);   
    ecrireStream(sd, "Bonjour, que désirez vous ?");    
    printf("Message envoyé au client: \"%s\"\n", "Bonjour, que désirez vous ?");

}

void dialogueAvecServeur(int sd){
    char buffer[MAX_BUFF];
    int nbOctets;
    PAUSE("PAUSE");
    ecrireStream(sd, "CECI EST UN TRES TRES TRES TRES LONG MESSAGE");
    lireStream(sd, buffer);

    printf("Réponse reçue du serveur : \"%s\"\n", buffer);

    
}