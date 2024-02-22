#include <drogon/drogon.h>
#include <iostream>

int main() {
    //Set HTTP listener address and port
    //get port from env
    const auto serverPort = std::stoi(getenv("SERVER_PORT"));
    const auto databaseHost = getenv("DATABASE_HOST");
    const auto databasePort = std::stoi(getenv("DATABASE_PORT"));
    const auto databaseName = getenv("DATABASE_NAME");
    const auto databaseUser = getenv("DATABASE_USER");
    const auto databasePassword = getenv("DATABASE_PASSWORD");

    drogon::app().addListener("0.0.0.0", serverPort);
    LOG_INFO << "Initializing app on port: " << serverPort;

    drogon::app().createDbClient("postgresql", databaseHost, databasePort, databaseName, databaseUser, databasePassword);

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
