#include "config.h"


/**
@brief Constants: Fields
*/
const QString Config::FIELD__ID                 = "ID";
const QString Config::FIELD__PORT               = "Port";
const QString Config::FIELD__CONN_MAX           = "ConnMax";
const QString Config::FIELD__CONN_PER_CLI       = "ConnPerCli";
const QString Config::FIELD__CONN_LIFE_TIME     = "ConnLifeTime";
const QString Config::FIELD__BLACK              = "Black";
const QString Config::FIELD__USE_LOG            = "UseLog";
const QString Config::FIELD__LOG                = "Log";
const QString Config::FIELD__WS_REPEATER        = "WsRepeater";


/**
@brief  Constructor.
@param  None.
@return None.
*/
Config::Config(QObject *parent) : QObject(parent)
{
    mLogUse  = false;
    mLogFile = QString("");

    this->byDefault();
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Config::~Config()
{
    this->byDefault();
}


/**
@brief  Public method: Set values by default.
@param  None.
@return None.
*/
void Config::byDefault()
{
    mID             = QString("");
    mPort           = 0;
    mConnMax        = 0;
    mConnPerCli     = 0;
    mConnLifeTime   = CONN_LIFE_TIME_OFF;
    mBlack          = QString("");
    mWsRepeater     = QString("");

    mListBlack.clear();
}


/**
@brief  Public method: Load values from JsonObject.
@param  DataIn - data.
@return True if value of options are correct, otherwise - False.
*/
bool Config::fromJson(const QJsonObject &DataIn)
{
    this->byDefault();

    if(!DataIn.isEmpty())
    {
        mID           = DataIn.value(FIELD__ID).toString(QString(""));
        mPort         = static_cast<quint16>(DataIn.value(FIELD__PORT).toInt(0));
        mConnMax      = static_cast<quint8>(DataIn.value(FIELD__CONN_MAX).toInt(0));
        mConnPerCli   = static_cast<quint8>(DataIn.value(FIELD__CONN_PER_CLI).toInt(0));
        mConnLifeTime = static_cast<qint32>(DataIn.value(FIELD__CONN_LIFE_TIME).toInt(CONN_LIFE_TIME_OFF));
        mBlack        = DataIn.value(FIELD__BLACK).toString(QString(""));
        mLogFile      = DataIn.value(FIELD__LOG).toString(QString(""));
        mWsRepeater   = DataIn.value(FIELD__WS_REPEATER).toString(QString(""));

        bool Boo = (DataIn.value(FIELD__USE_LOG).toInt(0));
        mLogUse = ((Boo) ? true : false);

        this->normilize();

        if(!mBlack.isEmpty()) this->readListBlack(mBlack);
    }

    QString LogBuff = QString();
    if(mLogUse) this->toString(LogBuff);
    Log::log(LogBuff, mLogFile, mLogUse);

    return (this->isCorrect());
}


/**
@brief  Public method: Pack values to String buffer.
@param  StringIn - link to string buffer.
@return None.
*/
void Config::toString(QString &StringIn)
{
    QString Correct    = QString(" (is correct)");
    QString NotCorrect = QString(" (is not correct)");

    StringIn+= QString("[ CONFIG ]\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__ID;
    StringIn+= QString(" = ");
    StringIn+= mID;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__PORT;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mPort);
    StringIn+= ((this->isCorrectPort()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__CONN_MAX;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mConnMax);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__CONN_PER_CLI;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mConnPerCli);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__CONN_LIFE_TIME;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mConnLifeTime);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__BLACK;
    StringIn+= QString(" = ");
    StringIn+= mBlack;
    StringIn+= QString("\r\n");

    StringIn+= QString("ListBlack.size() = %1").arg(QString::number(mListBlack.size()));
    StringIn+= QString("\r\n");

    QList<QString> Keys = mListBlack.keys();
    QString Key;
    for(int i=0; i<Keys.size(); i++)
    {
        Key = Keys.at(i);
        StringIn+= QString("client[%1] = %2:%3").arg(QString::number(i), Key, QString::number(mListBlack.value(Key)));
        StringIn+= QString("\r\n");
    }

    StringIn+= QString(" - ");
    StringIn+= FIELD__WS_REPEATER;
    StringIn+= QString(" = ");
    StringIn+= mWsRepeater;
    StringIn+= QString("\r\n");
}


/**
@brief  Public method: Check option "Port".
@param  None.
@return True if value of option "Port" is correct, otherwise - False.
*/
bool Config::isCorrectPort()
{
    return ((mPort > 0) ? true : false);
}


/**
@brief  Public method: Check options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Config::isCorrect()
{
    return ((this->isCorrectPort()) ? true : false);
}


/**
@brief  Public method: Normilize options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Config::normilize()
{
    if(mConnMax > CONN_MAX)    mConnMax    = CONN_MAX;
    if(mConnPerCli > CONN_MAX) mConnPerCli = CONN_MAX;
    if(mConnLifeTime < CONN_LIFE_TIME_OFF) mConnLifeTime = CONN_LIFE_TIME_OFF;
    if(mConnLifeTime > CONN_LIFE_TIME_MAX) mConnLifeTime = CONN_LIFE_TIME_MAX;

    return (this->isCorrect());
}


/**
@brief  Public method: Read config from a file.
@param  FileIn - path to a file.
@return True if value of options are correct, otherwise - False.
*/
bool Config::read(const QString &FileIn)
{
     QJsonDocument Doc = this->readJsonDoc(FileIn);
     QJsonObject Obj;

     if(Doc.isObject()) Obj = Doc.object();

     return (this->fromJson(Obj));
}


/**
@brief  Public method: Read list of forbidden clients.
@param  FileIn - path to a file.
@return The number of forbidden clients.
*/
quint16 Config::readListBlack(const QString &FileIn)
{
    QJsonDocument Doc = this->readJsonDoc(FileIn);
    QJsonArray Arr;

    if(Doc.isArray()) Arr = Doc.array();

    return (this->parseListBlack(Arr));
}


/**
@brief  Private Method: Read data from JSON-file.
@param  FileIn - path to file.
@return QJsonDocument.
*/
QJsonDocument Config::readJsonDoc(const QString &FileIn)
{
    QJsonDocument Doc;

    if(!FileIn.isEmpty())
    {
        QFile File(FileIn);

        if(File.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QJsonParseError Err;
            QString Data = File.readAll();

            Doc = QJsonDocument::fromJson(Data.toUtf8(), &Err);

            if(Err.error != QJsonParseError::NoError)
            {
                Log::log(QString("Config::readJsonDoc: Error parse the Json-file %1! %2").arg(FileIn, Err.errorString()), mLogFile, mLogUse);
            }

            File.close();
        }
        else
        {
            Log::log(QString("Config::readJsonDoc: Error open the file %1!").arg(FileIn), mLogFile, mLogUse);
        }
    }

    return (Doc);
}


/**
@brief  Public method: Parse list of forbidden clients.
@param  DataIn - data.
@return The number of forbidden clients.
*/
quint16 Config::parseListBlack(const QJsonArray &DataIn)
{
    if(!DataIn.isEmpty())
    {
        QJsonValue Val;
        QStringList StrParts;
        QString Key;
        bool Ok;

        for(int i=0; i<DataIn.size(); i++)
        {
            Val = DataIn.at(i);

            if(Val.isString())
            {
                StrParts = Val.toString(QString()).split(':');

                if(StrParts.size())
                {
                    Key = StrParts.at(0);
                    mListBlack[Key] = ((StrParts.size() > 1) ? static_cast<quint16>(StrParts.at(1).toInt(&Ok)) : static_cast<quint16>(0));
                }
            }
        }
    }

    return (mListBlack.size());
}
