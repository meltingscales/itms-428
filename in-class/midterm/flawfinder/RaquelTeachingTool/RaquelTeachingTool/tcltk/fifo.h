/***************************************************************************
                            fifo.h
                            ------
    Description         : Defines the constants used to identify the named
                          pipes (fifo) between the GUI and the DBMS.
    Duration            : Feb 2006 - May 2006
    Author              : Guillaume Renard, Universite de Bourgogne, France
                                            University of Northumbria, UK

 ***************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define CLIENT_TO_DBMS "/usr/users/Raquel/RAQUEL/RaquelDBMS/DBMS/fifo/ClientToDBMS.fifo"
#define DBMS_TO_CLIENT_ERROR "/usr/users/Raquel/RAQUEL/RaquelDBMS/DBMS/fifo/DBMSToClientError.fifo" /* Unused yet */
#define DBMS_TO_CLIENT_RESULT "/usr/users/Raquel/RAQUEL/RaquelDBMS/DBMS/fifo/DBMSToClientResult.fifo"
