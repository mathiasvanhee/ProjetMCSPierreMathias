#include "../session/session.h"
#include "../data/data.h"

int main(int argc, char const *argv[])
{
    socklen_t cltLen;
    struct sockaddr_in clt;
    int se, sd;

    se = creerSockAddrEcoute("127.0.0.1",5000, 5);
    while(1){
        cltLen = sizeof(struct sockaddr_in);
        sd = attenteAppel(se, &clt);
        creerProcessusServeur(se, sd, clt);
        close(sd);
    }
    close(se);
    
    return 0;
}


