#include "client.h"

void getConnectionData(struct sockaddr_storage &host, socklen_t &hostLen, int &hostSocket,
                       string hostname, int port, int socketType) {
    struct addrinfo hints, *res = NULL;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = socketType;
    hints.ai_protocol = 0;
    if (getaddrinfo(hostname.c_str(), intToString(port).c_str(), &hints, &res))
        die("Cannot resolve hostname " + hostname);
    memcpy(&host, res->ai_addr, res->ai_addrlen);
    hostLen = res->ai_addrlen;
    // Create socket
    if((hostSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
        freeaddrinfo(res);
        die("Cannot create socket");
    }
    freeaddrinfo(res);
}

void connectToGUI(ClientInfo &clientInfo, GuiInfo &guiInfo) {
    struct sockaddr_storage gui;
    socklen_t guiLen;
    getConnectionData(gui, guiLen, guiInfo.socket, clientInfo.uiHost, clientInfo.uiPort, SOCK_STREAM);
    guiInfo.ufds.fd = guiInfo.socket;
    guiInfo.ufds.events = POLLIN;
    // Turn off Nagle's algorithm to minimalize delay
    int flag = 1;
    int result = setsockopt(guiInfo.socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    if(result < 0)
        printError("Could not turn off Nagle's algortihm. I will continue working with Nagle's turn on.");
    if(guiInfo.socket == -1)
        die("Could not create socket for GUI connection");

    std::cout << "GUI: I want to connect to " << clientInfo.uiHost.c_str() << " (translated to "
              << "costam" << "):" << clientInfo.uiPort<< std::endl;

    if(connect(guiInfo.socket, (struct sockaddr*) &gui, guiLen) < 0) {
        die("Connecting to GUI failed.");
    }
}

void initializeSessionId(ClientData &clientData) {
    dataLock.lock();
    clientData.sessionId = getTime();
    dataLock.unlock();
}

void connectToServer(ClientInfo &clientInfo, ServerInfo &serverInfo) {
    getConnectionData(serverInfo.server, serverInfo.len, serverInfo.socket,
                      clientInfo.serverHost, clientInfo.serverPort, SOCK_DGRAM);
    serverInfo.ufds.fd = serverInfo.socket;
    serverInfo.ufds.events = POLLIN;
    // that's an UDP "connection" - since we connect only to the server, we save the address
    if(connect(serverInfo.socket, (struct sockaddr *) &serverInfo.server, serverInfo.len) < 0) {
        die("Connecting to server failed.");
    }
    std::cout << "Connecting to server at " << clientInfo.serverHost << ":" << clientInfo.serverPort
              << " successful." << std::endl;
}

bool readLineFromGui(int socket, string &line) {
    static string buffer;
    string::iterator pos;
    while((pos = find(buffer.begin(), buffer.end(), NEWLINE)) == buffer.end()) {
        char buff[BUFF_SIZE];
        ssize_t n = read(socket, buff, BUFF_SIZE);
        if(n == -1) {
            line = buffer;
            buffer = "";
            return false;
        }
        buff[n] = 0;
        buffer += buff;

    }
    // splitting into line and the rest of buffer
    line = string(buffer.begin(), pos);
    buffer = string(pos+1, buffer.end());
    return true;
}

void fillTurnDirection(GuiInfo &guiInfo) {
    if((guiInfo.leftPressed && guiInfo.rightPressed) || (!guiInfo.leftPressed && !guiInfo.rightPressed))
        clientData.turn_direction = 0;
    else if(guiInfo.leftPressed)
        clientData.turn_direction = -1;
    else if(guiInfo.rightPressed)
        clientData.turn_direction = 1;
}

void readFromGui(GuiInfo &guiInfo) {
    string line;
    if(!readLineFromGui(guiInfo.socket, line))
        die("Wrong message from GUI");
    std::cout << line << std::endl;
    if(line == turns::LEFT_DOWN)
        guiInfo.leftPressed = true;
    else if(line == turns::LEFT_UP)
        guiInfo.leftPressed = false;
    else if(line == turns::RIGHT_DOWN)
        guiInfo.rightPressed = true;
    else if(line == turns::RIGHT_UP)
        guiInfo.rightPressed = false;
    dataLock.lock();
    fillTurnDirection(guiInfo);
    dataLock.unlock();
}

void sendToGui(GuiInfo &guiInfo, Event event) {

    string message = event.toGuiRepr(game);
    if(message == "X")
        return;
    printf("I SEND TO GUI: %s", message.c_str());
    if(send(guiInfo.socket, message.c_str(), message.size()+1, 0) < 0)
        die("Error when communicating with GUI");
    //std::cout << "sent: " << message << " to gui" << std::endl;
}

void readFromServer(ServerInfo &serverInfo) {
    char buffer[RECV_BUFF_SIZE];
    bzero(buffer, RECV_BUFF_SIZE);
    int pos = 4;
    dataLock.lock();
    if(recv(serverInfo.socket, buffer, RECV_BUFF_SIZE, 0) < 0)
        die("Error while receiving message from server");

    uint32_t gameId;
    memcpy(&gameId, buffer, sizeof(uint32_t));
    gameId = ntohl(gameId);
    while(pos+10 < RECV_BUFF_SIZE) {
        Event event = Event::makeEvent(buffer, pos, game);
        if(event.len != 0 && event.number+1 >= clientData.next_expected_event_no) {
            eventsToGui.push(event);
            dataLock.lock();
            clientData.next_expected_event_no = std::max(clientData.next_expected_event_no, event.number);
            dataLock.unlock();
        }
    }
    dataLock.unlock();
}

void sendMessageToServer(ClientData &clientData, ServerInfo &serverInfo) {
    // std::cout << "Sending message: " << clientData.sessionId << ", " << (int)clientData.turn_direction
    //           << ", " << clientData.next_expected_event_no << ", " << clientData.playerName << std::endl;
    uint64_t time = htobe64(clientData.sessionId);
    int8_t turn_direction = clientData.turn_direction;
    uint32_t next_expected_event_no = htonl(clientData.next_expected_event_no);
    const char *player_name = clientData.playerName.c_str();
    size_t len = MIN_BUFF_SIZE + clientData.playerName.size();
    char *buffer = new char[len];
    bzero(buffer, len);
    memcpy(buffer, &time, 8);
    memcpy(buffer+8, &turn_direction, 1);
    memcpy(buffer+9, &next_expected_event_no, 4);
    memcpy(buffer+13, player_name, clientData.playerName.size()+1);
    if(send(serverInfo.socket, buffer, len, 0) < 0)
        die("Error while sending message to server. Probably server not working properly.");
    free(buffer);
}

void makeRelay() {
    dataLock.lock();
    sendMessageToServer(clientData, serverInfo);
    dataLock.unlock();
}

void timerStart() {
    unsigned interval = SEND_INTERVAL;
    std::function<void(void)> func = &makeRelay;
    std::thread([func, interval]() {
        while (running) {
            auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
            func();
            std::this_thread::sleep_until(x);
        }
    }).detach();
}

void handleConnections(GuiInfo &guiInfo, ServerInfo &serverInfo) {
    while(running) {
        int guiRv = poll(&guiInfo.ufds, 2, POLL_INTERVAL);
        if(guiRv > 0)
            readFromGui(guiInfo);
        int serverRv = poll(&serverInfo.ufds, 2, POLL_INTERVAL);
        if(serverRv > 0)
            readFromServer(serverInfo);
        while(!eventsToGui.empty()) {
            sendToGui(guiInfo, eventsToGui.front());
            //sleep(1);
            eventsToGui.pop();
        }
    }
}

int main(int argc, char **argv) {
    ClientInfo clientInfo = parseArguments(argc, argv);
    GuiInfo guiInfo(false, false);
    clientData = ClientData(clientInfo.playerName);
    // initializing and connecting to GUI and server
    initializeSessionId(clientData);
    connectToGUI(clientInfo, guiInfo);
    connectToServer(clientInfo, serverInfo);
    // thread that send messages to server every SEND_INTERVAL ms
    timerStart();
    handleConnections(guiInfo, serverInfo);
    int timeout(-1);
    return 0;
}