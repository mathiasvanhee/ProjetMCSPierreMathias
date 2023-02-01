#include "protocole.h"

void str_to_rep(char * serial, req_t * rep){
    char * token;
    int i;

    token = strtok(serial, ":");
    if(token == NULL){
        //rien n'est envoyé = fin du client
        rep->idReq = SOCKET_CLOSED;
        return;
    }
    rep->idReq = atoi(token);
    

    switch (rep->idReq)
    {
    case LISTE_INFOS:
        rep->r.reqListeInfos.taille = 10;
        rep->r.reqListeInfos.tabInfos = malloc(rep->r.reqListeInfos.taille * sizeof(infoListe_t));
        //On realloc dès qu'on ajoute un élément car la taille est variable (inconnue au départ) (ici par bloc de 10)
        for(i = 0;;i++){
            if(i>=rep->r.reqListeInfos.taille){
                rep->r.reqListeInfos.taille+=10;
                rep->r.reqListeInfos.tabInfos = realloc(rep->r.reqListeInfos.tabInfos, rep->r.reqListeInfos.taille * sizeof(infoListe_t));
            }
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
        rep->r.reqListeInfos.tabInfos = realloc(rep->r.reqListeInfos.tabInfos, rep->r.reqListeInfos.taille * sizeof(infoListe_t));
    break;

    case INFOS_DIFFUSION:
        token = strtok(serial, ":");
        strcpy(rep->r.reqInfosDiffusion.addrIP, token);

        token = strtok(serial, ":");
        rep->r.reqInfosDiffusion.port = atoi(token);

        token = strtok(serial, ":");
        strcpy(rep->r.reqInfosDiffusion.description, strtok(NULL, ":"));
    break;

    case DEMANDE_RETIRER_LISTE:
        rep->r.reqRetirerListe = atoi(strtok(NULL, ":"));
    break;

    case DEMANDE_LISTE:
        //Il n'y a que l'id de la requête, aucune information n'est demandée
        break;

    case DEMANDE_AJOUTER_LISTE:
        rep->r.reqAjouterListe.port = atoi(strtok(NULL, ":"));
        strcpy(rep->r.reqAjouterListe.description, strtok(NULL, ":"));
        break;


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
    case LISTE_INFOS:
        for (int i = 0; i < req->r.reqListeInfos.taille; i++)
        {
            sprintf(temp, ":%ld:%s", req->r.reqListeInfos.tabInfos[i].id, req->r.reqListeInfos.tabInfos[i].description);
            strcat(serial, temp);
        }
        break;

    case INFOS_DIFFUSION:
        sprintf(temp, ":%s:%d:%s", req->r.reqInfosDiffusion.addrIP, req->r.reqInfosDiffusion.port, req->r.reqInfosDiffusion.description);
        strcat(serial, temp);
        break;

    case DEMANDE_RETIRER_LISTE:
        sprintf(temp, ":%d", req->r.reqRetirerListe);
        strcat(serial, temp);
        break;

    case DEMANDE_LISTE:
        //Il n'y a que l'id de la requête, aucune information n'est demandée
        break;

    case DEMANDE_AJOUTER_LISTE:
        sprintf(temp, ":%d:%s", req->r.reqAjouterListe.port, req->r.reqAjouterListe.description);
        strcat(serial, temp);
        break;

    default:
        break;
    }
}

void initReqAjouterListe(req_t * req, int port, char * description){
    req->idReq = DEMANDE_AJOUTER_LISTE;
    strcpy(req->r.reqAjouterListe.description, description);
    req->r.reqAjouterListe.port = port;
}