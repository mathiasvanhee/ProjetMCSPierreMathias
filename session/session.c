#include "session.h"

int creerSocket(int type){
    int sock;
    if(type != SOCK_DGRAM && type != SOCK_STREAM){
        fprintf(stderr, "Type non supporté %d \n", type);
        exit(-1);
    }
    CHECK(sock = socket(PF_INET, type, 0), "Pb Creation Socket");
    return sock;
}

void adresserSocket(int sock, char * IPaddr, short port){
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    inet_aton(IPaddr, &addr.sin_addr);
    addr.sin_port = htons(port);
    memset(&addr.sin_zero, 0, 8);
    CHECK(bind(sock, (struct sockaddr *) &addr, sizeof(addr)), "Pb bind");
}

int creerSocketAddr(int type, char * IPaddr, short port){
    int sock = creerSocket(type);
    adresserSocket(sock, IPaddr, port);
    return sock;
}

int creerSockAddrEcoute(char * IPaddr, short port, int maxfile)
{
    int sock = creerSockAddrEcoute(SOCK_STREAM, IPaddr, port);
    CHECK(listen(sock, maxfile), "Pb listen");
    return sock;
}

int attenteAppel(int sockEcoute, struct sockaddr_in * pClt)
{
    int sockDialogue;
    int len = sizeof(pClt);
    CHECK(sockDialogue = accept(sockEcoute, (struct sockaddr *) pClt, &len), "Pb accept");
    return sockDialogue;
}

void connectSrv(int sock, char * IPaddr, short port)
{
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    inet_aton(IPaddr, &addr.sin_addr);
    addr.sin_port = htons(port);
    memset(&addr.sin_zero, 8, 0);
    CHECK(connect(sock, (struct sockaddr *)&addr, sizeof(addr)), "Pb-connect");
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
