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
    const auto logLevel = getenv("LOG_LEVEL");

    //print each env variable
    std::cout << "SERVER_PORT: " << serverPort << std::endl;
    std::cout << "DATABASE_HOST: " << databaseHost << std::endl;
    std::cout << "DATABASE_PORT: " << databasePort << std::endl;
    std::cout << "DATABASE_NAME: " << databaseName << std::endl;
    std::cout << "DATABASE_USER: " << databaseUser << std::endl;
    std::cout << "DATABASE_PASSWORD: " << databasePassword << std::endl;
    std::cout << "LOG_LEVEL: " << logLevel << std::endl;
    
    if (logLevel == "debug") drogon::app().setLogLevel(trantor::Logger::kDebug);
    else if (logLevel == "info") drogon::app().setLogLevel(trantor::Logger::kInfo);
    else if (logLevel == "warn") drogon::app().setLogLevel(trantor::Logger::kWarn);
    else if (logLevel == "error") drogon::app().setLogLevel(trantor::Logger::kError);
    else if (logLevel == "fatal") drogon::app().setLogLevel(trantor::Logger::kFatal);
    else drogon::app().setLogLevel(trantor::Logger::kInfo);

    drogon::app().addListener("0.0.0.0", serverPort);
    LOG_INFO << "Initializing app on port: " << serverPort;
    
    drogon::app().createDbClient("postgresql", databaseHost, databasePort, databaseName, databaseUser, databasePassword, 1,"","default",true);

    LOG_INFO << "Database connection created";

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

    LOG_INFO << "Starting app";
    drogon::app().run();

    return 0;
}
