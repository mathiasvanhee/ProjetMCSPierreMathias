
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#ifndef CHECK
#define CHECK(sts, msg) \
    if ((sts) == -1)    \
    {                   \
        perror(msg);    \
        exit(-1);       \
    }
#endif

#ifndef PAUSE
#define PAUSE(msg) printf("%s [touche]", msg); getchar();
#endif

/**
 * \brief Crée une socket
 * 
 * \param type      son type :  <tt>SOCK_DGRAM</tt> ou <tt>SOCK_STREAM</tt>
 * \return int      la socket créée
 */
int creerSocket(int type);

/**
 * \brief Remplit l'adresse locale de la socket
 * 
 * \param sock      socket
 * \param IPaddr    adresse IP
 * \param port      port
 * \return int      -1 s'il n'a pas réussi à l'adresser.
 */
int adresserSocket(int sock, char * IPaddr, short port);

/**
 * \brief Creer la socket et remplit l'adresse locale de celle-ci
 * 
 * \param type      type de socket :  <tt>SOCK_DGRAM</tt> ou <tt>SOCK_STREAM</tt>
 * \param IPaddr    adresse IP locale
 * \param port      port
 * \return int      La socket créée, -1 s'il n'a pas réussi.
 */
int creerSocketAddr(int type, char * IPaddr, short port);

/**
 * \brief Crée une socket d'écoute en mode stream
 * 
 * \param IPaddr    Adresse locale sur laquelle écouter.
 * \param port      Port d'écoute.
 * \param maxfile   Nombre maximum de client dans la file d'attente d'écoute.
 * \return int      La socket créée.
 */
int creerSockAddrEcoute(char * IPaddr, short port, int maxfile);

/**
 * \brief Attend un appel à la socket d'écoute créée à partir de <tt>creerSockAddrEcoute</tt>, et crée une socket de dialogue
 *  lorsqu'une connexion est établie.
 * 
 * \param sockEcoute    Socket d'écoute.
 * \param pClt          Pointeur sur l'adresse du client à récuperer.
 * \return int          La socket de dialogue créée à partir de la nouvelle connexion.
 */
int attenteAppel(int sockEcoute, struct sockaddr_in * pClt);

/**
 * \brief Se connecte à une adresse distante (serveur).
 * 
 * \param sock      Socket de dialogue avec le serveur.
 * \param IPaddr    Adresse IPv4 du serveur.
 * \param port      Port du serveur.
 */
void connectSrv(int sock, char * IPaddr, short port);

/**
 * \brief Initialise la structure sockaddr_in.
 * 
 * \param pAddr     Pointeur sur la structure sockaddr_in à initialiser.
 * \param IPaddr    Adresse IPv4 à utiliser.
 * \param port      Port à utiliser.
 */
void initAddr(struct sockaddr_in * pAddr, char * IPaddr, short port);

