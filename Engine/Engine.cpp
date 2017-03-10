
#include <Engine/Engine.h>
#include <Engine/Core/Defines.h>

#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP

#include <cstdio>

namespace engine
{
    void InitLogging(int argc, char *argv[])
    {
        // Delete old log files
        std::remove("logs/infolog.log");
        std::remove("logs/perflog.log");

        START_EASYLOGGINGPP(argc, argv);

        // Configure default logger
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.set(el::Level::Info, el::ConfigurationType::Format, "%level %msg");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Format, "%level %msg %fbase %line");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Format, "%level %msg");

        defaultConf.set(el::Level::Info, el::ConfigurationType::Filename, "logs/infolog.log");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Filename, "logs/infolog.log");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Filename, "logs/infolog.log");

#if DEPLOY
        defaultConf.set(el::Level::Info, el::ConfigurationType::ToStandardOutput, "true");
        defaultConf.set(el::Level::Error, el::ConfigurationType::ToStandardOutput, "false");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::ToStandardOutput, "false");
#endif

        el::Loggers::reconfigureLogger("default", defaultConf);

        // Configure performance logger
        el::Configurations performanceConf;
        performanceConf.setToDefault();
        performanceConf.set(el::Level::Info, el::ConfigurationType::Format, "%logger %msg");

        performanceConf.set(el::Level::Info, el::ConfigurationType::Filename, "logs/perflog.log");
        performanceConf.set(el::Level::Error, el::ConfigurationType::Filename, "logs/perflog.log");
        performanceConf.set(el::Level::Warning, el::ConfigurationType::Filename, "logs/perflog.log");

#if 1 //DEPLOY
        performanceConf.set(el::Level::Info, el::ConfigurationType::ToStandardOutput, "false");
        performanceConf.set(el::Level::Error, el::ConfigurationType::ToStandardOutput, "false");
        performanceConf.set(el::Level::Warning, el::ConfigurationType::ToStandardOutput, "false");
#endif

        el::Loggers::reconfigureLogger("performance", performanceConf);
    }
}

