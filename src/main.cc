#include <drogon/drogon.h>
#include <iostream>

int serverPort = 5555;

int main() {
    //Set HTTP listener address and port
    
    drogon::app().addListener("0.0.0.0", serverPort);
    LOG_INFO << "Initializing app on port: " << serverPort;
    auto returnLoad = drogon::app().loadConfigFile("./config.yaml");
    LOG_INFO << "Load data "<< returnLoad.getConnectionCount();

    LOG_INFO << "Config: " << drogon::app().getCustomConfig().asString();

auto &app = drogon::app();
app.registerBeginningAdvice([]() {
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
