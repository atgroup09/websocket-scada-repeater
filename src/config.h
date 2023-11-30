#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

#include "log.h"


/**
@brief Config.
*/
class Config : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit Config(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Config();


    /**
    Public constants
    */

    /**
    @brief Constants: Fields
    */
    static const QString FIELD__ID;
    static const QString FIELD__PORT;
    static const QString FIELD__CONN_MAX;
    static const QString FIELD__CONN_PER_CLI;
    static const QString FIELD__CONN_LIFE_TIME;
    static const QString FIELD__BLACK;
    static const QString FIELD__USE_LOG;
    static const QString FIELD__LOG;
    static const QString FIELD__WS_REPEATER;

    /**
    @brief Constants: Limites
    */
    static const quint8  CONN_MAX           = 250;
    static const qint32  CONN_LIFE_TIME_OFF = -1;
    static const qint32  CONN_LIFE_TIME_MIN = 0;
    static const qint32  CONN_LIFE_TIME_MAX = 65000;


    /**
    Public options
    */

    /**
    @brief Public option: Server ID
    */
    QString mID;

    /**
    @brief Public option: Port number
    */
    quint16 mPort;

    /**
    @brief Public option: Max number of connections
    */
    quint8 mConnMax;

    /**
    @brief Public option: Max number of connections per one client
    */
    quint8 mConnPerCli;

    /**
    @brief Public option: Life-time of one connection (seconds)
    @detailed -1 for unlimited life-time
    */
    qint32 mConnLifeTime;

    /**
    @brief Public option: List of forbidden clients (path to a file)
    */
    QString mBlack;

    /**
    @brief Public option: List of forbidden clients
    @detailed ["Client-IP"] = Client-Port
              ...
              * Port as 0 - All ports
    */
    QMap<QString, quint16> mListBlack;

    /**
    @brief Public option: URL of WS-repeater.
    */
    QString mWsRepeater;

    /**
    @brief Public option: Log-file.
    */
    QString mLogFile;

    /**
    @brief Public option: Log on\off.
    */
    bool mLogUse;



    /**
    Public methods
    */

    /**
    @brief  Public method: Set values by default.
    @param  None.
    @return None.
    */
    void byDefault();

    /**
    @brief  Public method: Load values from JsonObject.
    @param  DataIn - data.
    @return True if value of options are correct, otherwise - False.
    */
    bool fromJson(const QJsonObject &DataIn);

    /**
    @brief  Public method: Pack values to String buffer.
    @param  StringIn - link to string buffer.
    @return None.
    */
    void toString(QString &StringIn);

    /**
    @brief  Public method: Check option "Port".
    @param  None.
    @return True if value of option "Port" is correct, otherwise - False.
    */
    bool isCorrectPort();

    /**
    @brief  Public method: Check options.
    @param  None.
    @return True if value of options are correct, otherwise - False.
    */
    bool isCorrect();

    /**
    @brief  Public method: Normilize options.
    @param  None.
    @return True if value of options are correct, otherwise - False.
    */
    bool normilize();

    /**
    @brief  Public method: Read config from a file.
    @param  FileIn - path to a file.
    @return True if value of options are correct, otherwise - False.
    */
    bool read(const QString &FileIn);

    /**
    @brief  Public method: Read list of forbidden clients.
    @param  FileIn - path to a file.
    @return The number of forbidden clients.
    */
    quint16 readListBlack(const QString &FileIn);


private:

    /**
    Private methods
    */

    /**
    @brief  Private Method: Read data from JSON-file.
    @param  FileIn - path to file.
    @return QJsonDocument.
    */
    QJsonDocument readJsonDoc(const QString &FileIn);

    /**
    @brief  Public method: Parse list of forbidden clients.
    @param  DataIn - data.
    @return The number of forbidden clients.
    */
    quint16 parseListBlack(const QJsonArray &DataIn);

};

#endif // CONFIG_H
