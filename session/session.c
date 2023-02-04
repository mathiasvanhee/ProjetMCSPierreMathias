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

int adresserSocket(int sock, char * IPaddr, short port){
    struct sockaddr_in addr;
    initAddr(&addr, IPaddr, port);
    return bind(sock, (struct sockaddr *) &addr, sizeof(addr));
}

int creerSocketAddr(int type, char * IPaddr, short port){
    int sock = creerSocket(type);
    if(adresserSocket(sock, IPaddr, port) == -1) return -1;
    return sock;
}

int creerSockAddrEcoute(char * IPaddr, short port, int maxfile)
{
    int sock;
    if((sock = creerSocketAddr(SOCK_STREAM, IPaddr, port)) != -1)
        CHECK(listen(sock, maxfile), "Pb listen");
    return sock;
}

int attenteAppel(int sockEcoute, struct sockaddr_in * pClt)
{
    int sockDialogue;
    socklen_t len = sizeof(*pClt);
    CHECK(sockDialogue = accept(sockEcoute, (struct sockaddr *) pClt, &len), "Pb accept");
    return sockDialogue;
}

void connectSrv(int sock, char * IPaddr, short port)
{
    struct sockaddr_in addr;
    initAddr(&addr, IPaddr, port);
    CHECK(connect(sock, (struct sockaddr *)&addr, sizeof(addr)), "Pb-connect");
}


void initAddr(struct sockaddr_in * pAddr, char * IPaddr, short port)
{
    pAddr->sin_family = PF_INET;
    inet_aton(IPaddr, &pAddr->sin_addr);
    pAddr->sin_port = htons(port);
    memset(&pAddr->sin_zero, 0, 8);
}


