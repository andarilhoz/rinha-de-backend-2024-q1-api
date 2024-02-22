#include <drogon/drogon.h>
#include <iostream>

int serverPort = 8080;

int main() {
    //Set HTTP listener address and port
    //get port from env
    const char *envServerPort = std::getenv("SERVER_PORT");
    if (envServerPort != nullptr) {
        serverPort = std::stoi(envServerPort);
    }

    drogon::app().addListener("0.0.0.0", serverPort);
    LOG_INFO << "Initializing app on port: " << serverPort;
    drogon::app().loadConfigFile("./config.json");

    auto &app = drogon::app();
    app.registerBeginningAdvice([]() {
        auto &app = drogon::app();
        auto config = app.getCustomConfig();
        auto dbclient = drogon::app().getFastDbClient();
        dbclient->connectionInfo();
        std::cout << "Database connection: "
                << (dbclient == nullptr
                        ? "FAILED"
                        : dbclient->connectionInfo())
                << std::endl;
    });


    
        
    drogon::app().run();

    return 0;
}
