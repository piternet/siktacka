#include "client.h"

void connectToGUI(ClientInfo &clientInfo, int &guiSocket) {
    struct sockaddr_storage gui;
    char message[] = "NEW_GAME 800 600 piternet anon anonek";

    // Convert hostname to IP address
    struct addrinfo hints, *res = NULL;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC;

    if (getaddrinfo(clientInfo.uiHost.c_str(), intToString(clientInfo.uiPort).c_str(), &hints, &res))
        die("Cannot resolve hostname");
    memcpy(&gui, res->ai_addr, res->ai_addrlen);
    auto guiLen = res->ai_addrlen;

    // Create socket
    guiSocket = socket(res->ai_family, SOCK_STREAM, 0);
    // Turn off Nagle's algorithm to minimalize delay
    int flag = 1;
    int result = setsockopt(guiSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    if(result < 0)
        die("Could not turn off Nagle's algortihm");
    if(guiSocket == -1)
        die("Could not create socket for GUI connection");
    freeaddrinfo(res);

    std::cout << "GUI: I want to connect to " << clientInfo.uiHost.c_str() << " (translated to "
              << "costam" << "):" << clientInfo.uiPort<< std::endl;

    if(connect(guiSocket, (struct sockaddr*) &gui, guiLen) < 0) {
        die("Connecting to GUI failed.");
    }

    if(send(guiSocket, message, strlen(message), 0) < 0)
        die("Sending message to GUI failed.");
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

void readFromGui(GuiInfo &guiInfo) {
    string line;
    while(true) {
        if(!readLineFromGui(guiInfo.socket, line))
            die("Wrong message from GUI");
        std::cout << line << std::endl;
        guiLock.lock();
        if(line == turns::LEFT_DOWN)
            guiInfo.leftPressed = true;
        else if(line == turns::LEFT_UP)
            guiInfo.leftPressed = false;
        else if(line == turns::RIGHT_DOWN)
            guiInfo.rightPressed = true;
        else if(line == turns::RIGHT_UP)
            guiInfo.rightPressed = false;
        else
            die("Wrong message from GUI");
        guiLock.unlock();
    }

}

int main(int argc, char **argv) {
    ClientInfo clientInfo = parseArguments(argc, argv);
    GuiInfo guiInfo(false, false);
    connectToGUI(clientInfo, guiInfo.socket);
    // odpalam watek ktory co 20ms wysyla do sewera komunikat
    readFromGui(guiInfo);
    return 0;
}