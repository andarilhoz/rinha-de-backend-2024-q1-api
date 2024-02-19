#include <drogon/drogon.h>
#include <iostream>

int serverPort = 5555;

int main() {
    //Set HTTP listener address and port
    
    drogon::app().addListener("0.0.0.0", serverPort);
    LOG_INFO << "Initializing app on port: " << serverPort;
    drogon::app().loadConfigFile("/workspaces/rinha-de-backend-2024-q1-api/config.json");

    auto &app = drogon::app();
    app.registerBeginningAdvice([]() {
        auto &app = drogon::app();
        auto config = app.getCustomConfig();
        auto dbclient = drogon::app().getDbClient();
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
