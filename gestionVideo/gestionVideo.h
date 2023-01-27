#include <opencv2/opencv.hpp>

extern "C" {
    #include "../data/data.h"
    #include "../session/session.h"
}

using namespace cv;
using namespace std;

typedef struct infosConnexion
{
    int sock;
    struct sockaddr_in * addrSrc;
}infosConnexion_t;


/**
 * \brief       Diffuse la capture de vidéo à un client.
 * \param ptr   Adresse de la socket de dialogue avec le client.
 * \note        A utiliser en tant que thread .
 * \warning     La fonction demarrerVideo doit avoir été appelée avant.
 * \warning     la socket doit être bien initialisée
 */
extern "C" void *diffusion(void *ptr);

/**
 * \brief demarre la capture vidéo, et demande à l'utilisateur quel type de capture utiliser.
 * 
 */
extern "C" int demarrerVideo();

/**
 * \brief arrête la capture vidéo
 * 
 */
extern "C" int arreterVideo();

