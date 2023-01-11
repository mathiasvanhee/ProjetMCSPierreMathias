#include "../session/session.h"
#include "../data/data.h"

int main(int argc, char const *argv[])
{
    int sock = creerSocket(SOCK_STREAM);
    connectSrv(sock, "127.0.0.1", 5000);
    identificationClient(sock);
    close(sock);
    return 0;
}
