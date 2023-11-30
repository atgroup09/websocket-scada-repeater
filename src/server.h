#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QtWebSockets>

#include "global.h"
#include "log.h"
#include "config.h"
#include "client.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
QT_FORWARD_DECLARE_CLASS(QString)


/**
@brief WebSocket Server
*/
class Server : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  ConfigFileIn - path to config-file.
    @return None.
    */
    explicit Server(QString ConfigFileIn, QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Server();


    /**
    Public constants
    */


    /**
    Public methods
    */


signals:

    /**
    Signals
    */

    /**
    @brief  Signal: The server have started.
    @param  None.
    @return None.
    */
    void started();

    /**
    @brief  Signal: The server have stopped.
    @param  None.
    @return None.
    */
    void stopped();


public slots:

    /**
    Public slots
    */

    /**
    @brief  Public slot: Start the server.
    @param  None.
    @return true if the server started, otherwise - false.
    */
    bool start();

    /**
    @brief  Public slot: Stop the server.
    @param  None.
    @return true if the server stopped, otherwise - false.
    */
    bool stop();


private:

    /**
    Private options
    */

    /**
    @brief Path to Config file
    */
    QString mConfigFile;

    /**
    @brief Config
    */
    Config mConfig;

    /**
    @brief Clients
    */
    QList<Client *> mClients;

    /**
    @brief Clients marked to disconnect
    */
    QList<Client *> mClientsMarked;

    /**
    @brief WebSocketServer
    */
    QWebSocketServer *mWebSocketServer;

    /**
    @brief True if the server is connected to WS-repeater.
    */
    bool mIsConnToWsRepeater;


    /**
    Private methods
    */

    /**
    @brief  Private method: Apply filtering by black-list and ConnMax.
    @param  WebSocketIn - connected client (WebSocket).
    @return true - blocked, false - not blocked.
    */
    bool hasCliBlocked(QWebSocket *WebSocketIn);

    /**
    @brief  Private method: Check life-time.
    @param  ClientIn - connected client.
    @return true - expired, false - not expired.
    */
    bool hasCliTimeExpired(Client *ClientIn);

    /**
    @brief  Private method: Disconnect a client.
    @param  ClientIn - client to disconnect.
    @return None.
    */
    void disconnectCli(Client *ClientIn);

    /**
    @brief  Private method: Disconnect all client.
    @param  None.
    @return None.
    */
    void disconnectAllCli();

    /**
    @brief  Private method: Disconnect marked clients.
    @param  None.
    @return None.
    */
    void disconnectMarkedCli();

    /**
    @brief  Private method: Get client by WebSocket pointer.
    @param  WebSocketIn - connected client (WebSocket).
    @return Pointer to Client or nullptr.
    */
    Client *getCli(QWebSocket *WebSocketIn);

    /**
    @brief  Private method: Get the number of clients by IP.
    @param  HostIn - host address of a client.
    @return The number of connected clients.
    */
    quint32 size(QHostAddress HostIn);

    /**
    @brief  Private method: Connect to WS-repeater.
    @param  UrlIn - URL of WS-repeater (ex.: "ws://localhost:8100").
    @return None.
    */
    void connToWsRepeater(const QString &UrlIn);


private slots:

    /**
    Private slots
    */

    /**
    @brief  Private slot: Client connected.
    @param  None.
    @return None.
    */
    void cliConnected();

    /**
    @brief  Private slot: Client disconnected.
    @param  None.
    @return None.
    */
    void cliDisconnected();

    /**
    @brief  Private slot: Send message.
    @param  DataIn - data.
    @return None.
    */
    void sendMessage(const QString &DataIn);
};

#endif // SERVER_H
