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
    int sock = creerSocketAddr(SOCK_STREAM, IPaddr, port);
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
    addr.sin_family = PF_INET;
    inet_aton(IPaddr, &addr.sin_addr);
    addr.sin_port = htons(port);
    memset(&addr.sin_zero, 0, 8);
    CHECK(connect(sock, (struct sockaddr *)&addr, sizeof(addr)), "Pb-connect");
}





