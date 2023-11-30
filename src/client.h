#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDateTime>
#include <QtWebSockets>


/**
@brief Client.
*/
class Client : public QObject
{
    Q_OBJECT

public:

    explicit Client(QObject *parent = 0);
    ~Client();


    /**
    Public options
    */

    /**
    @brief Public option: True if the client is WS-repeater.
    */
    bool mIsWsRepeater;

    /**
    @brief Public option: Pointer to WebSocket.
    */
    QWebSocket *mWebSocket;


    /**
    Public methods
    */

    /**
    @brief  Public Method: Refresh Stamp of last activity.
    @param  None.
    @return None.
    */
    void refreshStampActivity();

    /**
    @brief  Public Method: Get value od Stamp of last activity.
    @param  None.
    @return Stamp of last activity.
    */
    QDateTime getStampActivity();

    /**
    @brief  Public Method: Get value od Stamp of last activity (+ additional seconds).
    @param  SecIn - additional seconds.
    @return Stamp of last activity + SecIn.
    */
    QDateTime getStampActivity(const qint16 SecIn);

private:

    /**
    Private options
    */

    /**
    @brief Private option: DateTime of last activity.
    */
    QDateTime mStampActivity;
};

#endif // CLIENT_H
