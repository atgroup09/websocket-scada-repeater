#include "server.h"


/**
@brief  Static function: Get ID of connected peer.
@param  PeerIn - connected peer (socket).
@return ID as "IP:port" or empty QString
*/
static QString getPeerID(QWebSocket *PeerIn)
{
    return ((PeerIn) ? (QStringLiteral("%1:%2").arg(PeerIn->peerAddress().toString(), QString::number(PeerIn->peerPort()))) : QString());
}


/**
@brief  Constructor.
@param  ConfigFileIn - path to config-file.
@return None.
*/
Server::Server(QString ConfigFileIn, QObject *parent) : QObject(parent)
{
    mConfigFile         = ConfigFileIn;
    mWebSocketServer    = nullptr;
    mIsConnToWsRepeater = false;
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Server::~Server()
{
    this->stop();
}


/**
@brief  Public slot: Start the server.
@param  None.
@return true if the server started, otherwise - false.
*/
bool Server::start()
{
    mWebSocketServer = new QWebSocketServer(SERVER_NAME, QWebSocketServer::NonSecureMode, this);

    mConfig.read(mConfigFile);
    if(!mConfig.mID.isEmpty()) mWebSocketServer->setServerName(mConfig.mID);

    Log::log(QString("<START>"), mConfig.mLogFile, mConfig.mLogUse);

    if(mWebSocketServer->listen(QHostAddress::Any, mConfig.mPort))
    {
        Log::log(QString("The server %1 is listening on port %2").arg(mWebSocketServer->serverName(), QString::number(mWebSocketServer->serverPort())), mConfig.mLogFile, mConfig.mLogUse);
        connect(mWebSocketServer, &QWebSocketServer::newConnection, this, &Server::cliConnected);

        emit started();

        return (true);
    }

    return (false);
}


/**
@brief  Public slot: Stop the server.
@param  None.
@return true if the server stopped, otherwise - false.
*/
bool Server::stop()
{
    if(mWebSocketServer) mWebSocketServer->pauseAccepting();

    this->disconnectMarkedCli();
    this->disconnectAllCli();

    Log::log(QString("</START>"), mConfig.mLogFile, mConfig.mLogUse);

    if(mWebSocketServer)
    {
        delete mWebSocketServer;
        mWebSocketServer = nullptr;
    }

    emit stopped();

    return (true);
}


/**
@brief  Private method: Apply filtering by black-list and ConnMax.
@param  WebSocketIn - connected client (WebSocket).
@return true - blocked, false - not blocked.
*/
bool Server::hasCliBlocked(QWebSocket *WebSocketIn)
{
    if(WebSocketIn && mClients.size() < mConfig.mConnMax)
    {
        if(this->size(WebSocketIn->peerAddress()) < mConfig.mConnPerCli)
        {
            QString Key = WebSocketIn->peerAddress().toString();

            if(mConfig.mListBlack.contains(Key))
            {
                quint16 BloPort = mConfig.mListBlack.value(Key, (quint16)0);
                quint16 CliPort = WebSocketIn->peerPort();
                if(BloPort == 0 || BloPort == CliPort) return (true);
            }

            return (false);
        }
    }

    return (true);
}


/**
@brief  Private method: Check life-time.
@param  ClientIn - connected client.
@return true - expired, false - not expired.
*/
bool Server::hasCliTimeExpired(Client *ClientIn)
{
    if(ClientIn && mConfig.mConnLifeTime > Config::CONN_LIFE_TIME_MIN)
    {
        QDateTime CurrentStamp = QDateTime::currentDateTime();
        QDateTime ClientStamp  = ClientIn->getStampActivity(mConfig.mConnLifeTime);
        if(ClientStamp <= CurrentStamp) return (true);
    }

    return (false);
}


/**
@brief  Private method: Disconnect client.
@param  ClientIn - client.
@return None.
@detailed The client will be disconnect and remove from the List of clients.
          The memory, allocated to object of the client will be free.
*/
void Server::disconnectCli(Client *ClientIn)
{
    if(ClientIn && !mClients.isEmpty())
    {
        if(ClientIn->mIsWsRepeater) mIsConnToWsRepeater = false;
        mClients.removeAll(ClientIn);
        delete ClientIn;
    }
}


/**
@brief  Private method: Disconnect all client.
@param  None.
@return None.
*/
void Server::disconnectAllCli()
{
    if(!mClients.isEmpty())
    {
        Client *pClient = nullptr;

        while(!mClients.isEmpty())
        {
            pClient = mClients.takeLast();
            if(pClient)
            {
                if(pClient->mIsWsRepeater) mIsConnToWsRepeater = false;
                delete pClient;
            }
        }
    }
}


/**
@brief  Private method: Disconnect marked clients.
@param  None.
@return None.
*/
void Server::disconnectMarkedCli()
{
    if(!mClientsMarked.isEmpty() && !mClients.isEmpty())
    {
        Client *pClient = nullptr;

        while(!mClientsMarked.isEmpty())
        {
            pClient = mClientsMarked.takeLast();
            this->disconnectCli(pClient);
        }
    }
}


/**
@brief  Private method: Get client by WebSocket pointer.
@param  WebSocketIn - connected client (WebSocket).
@return Pointer to Client or nullptr.
*/
Client *Server::getCli(QWebSocket *WebSocketIn)
{
    if(WebSocketIn && !mClients.isEmpty())
    {
        Client *pClient = nullptr;

        for(int i=0; i<mClients.size(); i++)
        {
            pClient = mClients.at(i);

            if(pClient)
            {
                 if(pClient->mWebSocket == WebSocketIn) return (pClient);
            }
        }
    }

    return (nullptr);
}


/**
@brief  Private method: Get the number of clients by IP.
@param  HostIn - host address of a client.
@return The number of connected clients.
*/
quint32 Server::size(QHostAddress HostIn)
{
    quint32 Num = 0;

    if(!mClients.isEmpty())
    {
        QHostAddress Host;
        Client *pClient = nullptr;

        for(int i=0; i<mClients.size(); i++)
        {
            pClient = mClients.at(i);
            if(pClient)
            {
                Host = pClient->mWebSocket->peerAddress();
                if(Host.toString() == HostIn.toString()) Num++;
            }
        }
    }

    return (Num);
}


/**
@brief  Private slot: Client connected.
@param  None.
@return None.
*/
void Server::cliConnected()
{
    Log::log(QString("Server::cliConnected()"), mConfig.mLogFile, mConfig.mLogUse);

    Client *pClient = new Client();
    pClient->mWebSocket = mWebSocketServer->nextPendingConnection();

    if(!this->hasCliBlocked(pClient->mWebSocket))
    {
        pClient->mWebSocket->setParent(this);

        connect(pClient->mWebSocket, &QWebSocket::textMessageReceived, this, &Server::sendMessage);
        connect(pClient->mWebSocket, &QWebSocket::disconnected, this, &Server::cliDisconnected);

        mClients << pClient;

        Log::log(QString("%1 has connected").arg(getPeerID(pClient->mWebSocket)), mConfig.mLogFile, mConfig.mLogUse);
        Log::log(QString("clients = %1").arg(QString::number(mClients.size())), mConfig.mLogFile, mConfig.mLogUse);
    }
    else
    {
        Log::log(QString("%1 has blocked by black-list").arg(getPeerID(pClient->mWebSocket)), mConfig.mLogFile, mConfig.mLogUse);
        delete pClient;
    }
}


/**
@brief  Private slot: Client disconnected.
@param  None.
@return None.
*/
void Server::cliDisconnected()
{
    Log::log(QString("Server::cliDisconnected()"), mConfig.mLogFile, mConfig.mLogUse);

    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    Client *pClient = this->getCli(pSender);

    if(pClient)
    {
        Log::log(QString("%1 has disconnected").arg(getPeerID(pClient->mWebSocket)), mConfig.mLogFile, mConfig.mLogUse);
        this->disconnectCli(pClient);
        Log::log(QString("clients = %1").arg(QString::number(mClients.size())), mConfig.mLogFile, mConfig.mLogUse);
    }
}


/**
@brief  Private method: Connect to WS-repeater.
@param  UrlIn - URL of WS-repeater (ex.: "ws://localhost:8100").
@return None.
*/
void Server::connToWsRepeater(const QString &UrlIn)
{
    Log::log(QString("Server::connToWsRepeater(%1)").arg(UrlIn), mConfig.mLogFile, mConfig.mLogUse);

    if(!UrlIn.isEmpty())
    {
        Client *pClient = new Client();

        pClient->mIsWsRepeater = true;
        pClient->mWebSocket    = new QWebSocket(UrlIn);
        pClient->mWebSocket->setParent(this);
        connect(pClient->mWebSocket, &QWebSocket::disconnected, this, &Server::cliDisconnected);

        mClients << pClient;

        mIsConnToWsRepeater = pClient->mIsWsRepeater;

        pClient->mWebSocket->open(QUrl(UrlIn));
    }
    else
    {
        Log::log(QString("The URL of WS-repeater is empty!"), mConfig.mLogFile, mConfig.mLogUse);
    }
}


/**
@brief  Private slot: Send message.
@param  DataIn - data.
@return None.
*/
void Server::sendMessage(const QString &DataIn)
{
    Log::log(QString("Server::sendMessage()"), mConfig.mLogFile, mConfig.mLogUse);

    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());

    if(!mIsConnToWsRepeater) connToWsRepeater(mConfig.mWsRepeater);

    if(!mClients.isEmpty())
    {
        if(!DataIn.isEmpty())
        {
            Client *pClient = nullptr;
            int Size = mClients.size();

            for(int i=0; i<Size; i++)
            {
                pClient = mClients.at(i);

                if(pClient)
                {
                    if(pClient->mWebSocket != pSender)
                    {
                        if(!this->hasCliTimeExpired(pClient))
                        {
                            pClient->mWebSocket->sendTextMessage(DataIn);
                        }
                        else
                        {
                            mClientsMarked.append(pClient);
                        }
                    }
                }
            }

            this->disconnectMarkedCli();
        }
        else
        {
            Log::log(QString("The data to send is empty!"), mConfig.mLogFile, mConfig.mLogUse);
        }
    }
    else
    {
        Log::log(QString("The list of clients is empty!"), mConfig.mLogFile, mConfig.mLogUse);
    }
}
