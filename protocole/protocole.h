#include "../data/data.h"

#define MAX_DESC 60

/**
 * \brief  Informations d'une diffusion.
 */
typedef struct infosDiffusion{
    long id;
    char addrIP[16];
    int port;
    char description[MAX_DESC];
} infosDiffusion_t;

/**
 * \brief Informations d'une diffusion au sein de la liste affichée au le client
 * 
 */
typedef struct infoListe{
    long id;
    char description[MAX_DESC];
} infoListe_t;

/**
 * \brief Liste des informations de diffusion affichée au le client
 * 
 */
typedef struct listeInfos{
    int taille;
    infoListe_t * tabInfos;
}listeInfos_t;

/**
 * \brief Demande d'un client à se retirer de la liste des diffusions.
 * \remark Utilisée lorsqu'un client finit sa diffusion.
 * 
 */
typedef enum demandeRetirerListe{
    REASON_DEFAULT,
    REASON_ERROR
} demandeRetirerListe_t;

/**
 * \brief Demande d'ajout à la liste de diffusions d'un client vers le serveur central.
 * \remark Utilisée lorsqu'un client démarre une diffusion.
 * \note Pour la demande de nouvelle diffusion, on a juste besoin du port et de la description,
 * l'adresse du diffuseur est determinée par le serveur central, et son id est aussi choisie par le serveur central.
 */
typedef struct demandeAjouterListe{
    int port;
    char description[MAX_DESC];
} demandeAjouterListe_t;

/**
 * \brief Requête de demande des informations de diffusion d'un client vers le serveur central.
 * 
 */
typedef struct demandeInfosDiffusion{
    long id;
} demandeInfosDiffusion_t;


/**
 * \brief Identifiant d'une requête selon son type.
 * 
 */
typedef enum idReq{
    UNDEFINED,              ///Requête indéfinie (à ne pas utiliser)
    LISTE_INFOS,            ///Réponse du serveur principal à la demande de la liste des diffusions ouvertes       
    INFOS_DIFFUSION,        ///Réponse du serveur principal à la demande des informations de connexion à une diffusion
    DEMANDE_RETIRER_LISTE,  ///Demande d'arrêt d'une diffusion d'un client
    DEMANDE_LISTE,          ///Demande de la liste des diffusions ouvertes
    DEMANDE_AJOUTER_LISTE,  ///Demande de nouvelle diffusion
    DEMANDE_INFOS_DIFFUSION,///Demande les informations d'une diffusion d'après son id
    PORT_DGRAM,             ///Requete pour informer du port de la socket de diffusion datagram.
    SOCKET_CLOSED,          ///La socket a été closed
    BAD_REQUEST,            ///Réponse du serveur lorsqu'un client a envoyé une requête erronée au serveur (données reçues incohérentes)
    SUCCESS,                ///Réponse du serveur signifiant que la requête a été traitée avec succès. 
    ERROR                   ///Réponse du serveur signifiant qu'il y a eu une erreur lors du traitement de la requête.
} idReq_t;

/**
 * \brief Structure général d'une requête
 * 
 */
typedef struct req{
    idReq_t idReq;
    union req_union{
        listeInfos_t repListeInfos;
        infosDiffusion_t repInfosDiffusion;
        demandeRetirerListe_t reqRetirerListe;
        demandeAjouterListe_t  reqAjouterListe;
        demandeInfosDiffusion_t reqInfosDiffusion;
        int portDgram; //idReq = PORT_DGRAM
    } r;
} req_t;

/**
 * \brief Transforme une chaine de caractère en requête
 * 
 * \param serial La chaine de caractère (celle qui est envoyé sur le réseau)
 * \param rep    La requête qui est remplie sur la base de la chaine <tt>serial</tt>
 */
void str_to_rep(char * serial, req_t * rep);

/**
 * \brief Transforme une requête en chaine de caractère.
 * 
 * \param req    La requête.
 * \param serial La chaine de caractère qui est remplie (celle qui est envoyé sur le réseau)
 */
void req_to_str(req_t * req, char * serial);

/**
 * \brief initialise une requête de type <tt>DEMANDE_AJOUTER_LISTE</tt>
 * 
 * \param req           adresse de la requête à initialiser
 * \param port          valeur du port à initialiser
 * \param description   valeur de la  description à initialiser
 */
void initReqAjouterListe(req_t * req, int port, char * description);

/**
 * \brief initialise une requête de type <tt>DEMANDE_RETIRER_LISTE</tt>
 * 
 * \param req       adresse de la requête à initialiser
 * \param raison    valeur de la raison à initialiser
 */
void initReqRetirerListe(req_t * req, demandeRetirerListe_t raison);

/**
 * \brief initialise une requête de type <tt>DEMANDE_LISTE</tt>
 * 
 * \param req   adresse de la requête à initialiser
 */
void initReqDemandeListe(req_t * req);

/**
 * \brief initialise une requête de type <tt>DEMANDE_INFOS_DIFFUSION</tt>
 * 
 * \param req       adresse de la requête à initialiser
 * \param idDiff    valeur de l'identifiant de diffusion <tt>idDiff</tt> à initialiser
 */
void initReqDemInfosDiff(req_t * req, long idDiff);
