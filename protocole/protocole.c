#include "protocole.h"

void str_to_rep(char * serial, req_t * rep){
    char * token;
    int i;

    token = strtok(serial, ":");
    rep->idReq = atoi(token);
    

    switch (rep->idReq)
    {
    case 1:
        rep->r.reqListeInfos.tabInfos = malloc(rep->r.reqListeInfos.taille * sizeof(infoListe_t));
        for(i = 0;;i++){
            token=strtok(NULL, ":");
            if(token == NULL){
                    break;
            }
            rep->r.reqListeInfos.tabInfos[i].id = atoi(token);
            token=strtok(NULL, ":");
            if(token == NULL){
                    break;
            }
            strcpy(rep->r.reqListeInfos.tabInfos[i].description, token);
        }
        rep->r.reqListeInfos.taille=i;
    break;

    case 2:
        strcpy(rep->r.reqInfosDiffusion.addrIP, strtok(NULL, ":"));
        rep->r.reqInfosDiffusion.port = atoi(strtok(NULL, ":"));
        strcpy(rep->r.reqInfosDiffusion.description, strtok(NULL, ":"));
    break;

    case 3:
        rep->r.reqRetirerListe = atoi(strtok(NULL, ":"));
    break;

    case 4:

    break;

    default:
        break;
    }
}

void req_to_str(req_t * req, char * serial){
    char temp[MAX_BUFF];
    serial[0] = '\0';
    sprintf(serial, "%d", req->idReq);
    
    switch (req->idReq)
    {
    case 1:
        for(int i = 0; i < req->r.reqListeInfos.taille; i++){
            sprintf(temp, ":%ld:%s", req->r.reqListeInfos.tabInfos[i].id, req->r.reqListeInfos.tabInfos[i].description);
            strcat(serial, temp);
        }
    break;

    case 2:
        sprintf(temp, ":%s:%d:%s", req->r.reqInfosDiffusion.addrIP, req->r.reqInfosDiffusion.port, req->r.reqInfosDiffusion.description);
        strcat(serial, temp);
    break;

    case 3:
        sprintf(temp, ":%d", req->r.reqRetirerListe);
        strcat(serial, temp);
    break;

    default:
        break;
    }
}