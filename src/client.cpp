#include "client.h"


/**
@brief  Constructor.
@param  None.
@return None.
*/
Client::Client(QObject *parent) : QObject(parent)
{
    mIsWsRepeater = false;
    mWebSocket    = NULL;
    this->refreshStampActivity();
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Client::~Client()
{
    if(mWebSocket != NULL)
    {
        mWebSocket->close(QWebSocketProtocol::CloseCodeNormal);
        mWebSocket->deleteLater();
    }
}


/**
@brief  Public Method: Refresh Stamp of Activity.
@param  None.
@return None.
*/
void Client::refreshStampActivity()
{
    mStampActivity = QDateTime::currentDateTime();
}


/**
@brief  Public Method: Get value od Stamp of last activity.
@param  None.
@return QDateTime.
*/
QDateTime Client::getStampActivity()
{
    return (mStampActivity);
}


/**
@brief  Public Method: Get value od Stamp of last activity (+ additional seconds).
@param  SecIn - additional seconds.
@return Stamp of last activity + SecIn.
*/
QDateTime Client::getStampActivity(const qint16 SecIn)
{
    return (mStampActivity.addSecs(SecIn));
}
