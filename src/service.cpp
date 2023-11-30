#include "service.h"


/**
@brief Input arguments.
*/
const QString Service::ARG_KEY__CONFIG   = QString("--config");
const QString Service::ARG_KEY__SNAME    = QString("--sname");
const QString Service::ARG_KEY__SDESC    = QString("--sdesc");


/**
@brief  Constructor.
@param  argc  - size of argv;
        argv  - list of input arguments;
        sname - service name.
@return None.
*/
Service::Service(int argc, char *argv[], const QString sname) : QtService<QCoreApplication>(argc, argv, sname)
{
    QHash<QString, QString> ParsedArgs;
    Args::parse(argc, argv, ParsedArgs);

    mServer = new Server(ParsedArgs.value(ARG_KEY__CONFIG, QString("")));

    setServiceDescription(sname);
    setServiceFlags(QtServiceBase::CanBeSuspended);
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Service::~Service()
{
    delete mServer;
}


/**
@brief  Start the service.
@detailed this method is called by click on button Start in Service Manager.
@param  None.
@return None.
*/
void Service::start()
{
    QString SName = serviceName();
    QtServiceController controller(SName);
    controller.start();

    if(controller.isRunning())
    {
        mServer->start();
    }
}


/**
@brief  Stop the service.
@detailed this method is called by click on button Stop in Service Manager.
@param  None.
@return None.
*/
void Service::stop()
{
    QString SName = serviceName();
    QtServiceController controller(SName);
    controller.stop();

    mServer->stop();
}


/**
@brief  Pause the service.
@detailed this method is called by click on button Pause in Service Manager.
@param  None.
@return None.
*/
void Service::pause()
{
    QString SName = serviceName();
    QtServiceController controller(SName);
    controller.pause();

    mServer->stop();
}


/**
@brief  Resume the service.
@detailed this method is called by click on button Resume in Service Manager.
@param  None.
@return None.
*/
void Service::resume()
{
    QString SName = serviceName();
    QtServiceController controller(SName);
    controller.resume();

    mServer->start();
}
