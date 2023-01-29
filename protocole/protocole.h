#include "../data/data.h"

typedef struct infosDiffusion{
    long id;
    char addrIP[16];
    int port;
    char description[MAX_BUFF];
} infosDiffusion_t;

typedef struct infoListe{
    long id;
    char description[50];
} infoListe_t;

typedef struct listInfos{
    int taille;
    infoListe_t * tabInfos;
}listeInfos_t;

typedef struct demandeListe{
}demandeListe_t;

typedef enum{
    DEFAULT,
    ERROR
} demandeRetirerListe_t;

typedef struct{
    int idReq;
    union{
        listeInfos_t reqListeInfos; //idReq = 1; à malloc
        infosDiffusion_t reqInfosDiffusion;//idReq = 2
        demandeRetirerListe_t reqRetirerListe;//idReq = 3
        demandeListe_t  reqDemandeListe;//idReq = 4
    } r;
} req_t;

void str_to_rep(char *, req_t *);
void req_to_str(req_t *, char *);