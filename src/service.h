#ifndef SERVICE_H
#define SERVICE_H

#include <iostream>
#include <QCoreApplication>
#include <QString>
#include <qtservice.h>

#include "global.h"
#include "args.h"
#include "server.h"


/**
    Installation on Windows:

        sc create "<ServiceName>" binpath= "<Path to Exe-file with input arguments>" DisplayName= "<Service Display Name>" start= <startType: auto|demand|delayed-auto|disabled>

    Service arguments:

        --config Path-to-Config-file [ --sname ServiceName --sdesc ServiceDescription ]

        [  ] - optional arguments

        if argument --sname is not contains in the list of arguments, then used service name by default (SERVICE_NAME).
        if argument --sdesc is not contains in the list of arguments, then used service name by default (SERVICE_DESC).
*/


/**
@brief Service class.
*/
class Service : public QtService<QCoreApplication>
{

public:

    /**
    @brief  Constructor.
    @param  argc  - size of argv;
            argv  - list of input arguments;
            sname - service name.
    @return None.
    */
    Service(int argc, char *argv[], const QString sname);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    ~Service();


    /**
    Public options
    */

    /**
    @brief Input arguments.
    */
    static const QString ARG_KEY__CONFIG;
    static const QString ARG_KEY__SNAME;
    static const QString ARG_KEY__SDESC;


    /**
    Public methods
    */

    /**
    @brief  Start the service.
    @detailed this method is called by click on button Start in Service Manager.
    @param  None.
    @return None.
    */
    void start();

    /**
    @brief  Stop the service.
    @detailed this method is called by click on button Stop in Service Manager.
    @param  None.
    @return None.
    */
    void stop();

    /**
    @brief  Pause the service.
    @detailed this method is called by click on button Pause in Service Manager.
    @param  None.
    @return None.
    */
    void pause();

    /**
    @brief  Resume the service.
    @detailed this method is called by click on button Resume in Service Manager.
    @param  None.
    @return None.
    */
    void resume();


private:

    /**
    Private options
    */

    /**
    @brief Server.
    */
    Server *mServer;
};

#endif // SERVICE_H
