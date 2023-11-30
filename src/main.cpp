#include <QCoreApplication>

#include "global.h"
#include "args.h"
#include "service.h"


/**
    Input arguments:

    --config PathToConfigFile [ --sname ServiceName --sdesc ServiceDescription ]

    [  ] - optional arguments
*/

int main(int argc, char *argv[])
{
    QHash<QString, QString> ParsedArgs;
    Args::parse(argc, argv, ParsedArgs);

#ifdef SERVICE
    QString SName = ParsedArgs.value(Service::ARG_KEY__SNAME, QString(SERVICE_NAME));
    Service sc(argc, argv, SName);
    return sc.exec();
#else
    QCoreApplication a(argc, argv);
    QString ConfigFile = ParsedArgs.value(QString(ARG_KEY_CONFIG), QString(""));
    Server Srv(ConfigFile);
    Srv.start();
    return a.exec();
#endif
}
