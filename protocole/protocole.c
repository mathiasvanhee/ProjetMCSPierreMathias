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
        // On realloc dès qu'on ajoute un élément car la taille est variable (inconnue au départ) (ici par bloc de 10)
        for (i = 0;; i++)
        {
            if (i >= rep->r.reqListeInfos.taille)
            {
                rep->r.reqListeInfos.taille += 10;
                rep->r.reqListeInfos.tabInfos = realloc(rep->r.reqListeInfos.tabInfos, rep->r.reqListeInfos.taille * sizeof(infoListe_t));
            }
            token = strtok(NULL, ":");
            if (token == NULL)
            {
                break;
            }
            rep->r.reqListeInfos.tabInfos[i].id = atoi(token);
            token = strtok(NULL, ":");
            if (token == NULL)
            {
                break;
            }
            strcpy(rep->r.reqListeInfos.tabInfos[i].description, token);
        }
        rep->r.reqListeInfos.taille = i;
        rep->r.reqListeInfos.tabInfos = realloc(rep->r.reqListeInfos.tabInfos, rep->r.reqListeInfos.taille * sizeof(infoListe_t));
        break;

    case INFOS_DIFFUSION:
        token = strtok(NULL, ":");
        if (token == NULL)
        {
            rep->idReq = BAD_REQUEST;
            return;
        }
        strcpy(rep->r.repInfosDiffusion.addrIP, token);

        token = strtok(NULL, ":");
        if (token == NULL)
        {
            rep->idReq = BAD_REQUEST;
            return;
        }
        rep->r.repInfosDiffusion.port = atoi(token);

        token = strtok(NULL, ":");
        if (token == NULL)
        {
            rep->idReq = BAD_REQUEST;
            return;
        }
        strcpy(rep->r.repInfosDiffusion.description, token);
        break;

    case DEMANDE_RETIRER_LISTE:
        token = strtok(NULL, ":");
        if (token == NULL)
        {
            rep->idReq = BAD_REQUEST;
            return;
        }
        rep->r.reqRetirerListe = atoi(token);
        break;

    case DEMANDE_AJOUTER_LISTE:
        token = strtok(NULL, ":");
        if (token == NULL)
        {
            rep->idReq = BAD_REQUEST;
            return;
        }
        rep->r.reqAjouterListe.port = atoi(token);

        token = strtok(NULL, ":");
        if (token == NULL)
        {
            rep->idReq = BAD_REQUEST;
            return;
        }
        strcpy(rep->r.reqAjouterListe.description, token);

        break;
    case DEMANDE_INFOS_DIFFUSION:
        token = strtok(NULL, ":");
        if (token == NULL)
        {
            rep->idReq = BAD_REQUEST;
            return;
        }
        rep->r.reqInfosDiffusion.id = atoi(token);
        break;
    //toutes les requêtes qui n'ont pas besoin de paramètres : 
    case DEMANDE_LISTE:
    case BAD_REQUEST:
    case SUCCESS:
    case ERROR:
        break;

    default: //pour toutes les requêtes non traitées, on considère que c'est une BAD_REQUEST
        rep->idReq = BAD_REQUEST;
        return;
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
        sprintf(temp, ":%s:%d:%s", req->r.repInfosDiffusion.addrIP, req->r.repInfosDiffusion.port, req->r.repInfosDiffusion.description);
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

    case DEMANDE_INFOS_DIFFUSION:
        sprintf(temp, ":%ld", req->r.reqInfosDiffusion.id);
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

void initReqRetirerListe(req_t * req, demandeRetirerListe_t raison){
    req->idReq = DEMANDE_RETIRER_LISTE;
    req->r.reqRetirerListe = raison;
}