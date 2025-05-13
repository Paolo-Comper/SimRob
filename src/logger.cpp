#include "logger.hpp"
#include "settings.hpp"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// inizzializza il logger in mdo che le impostazioni siano globali

void Logger::Init() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

#if LOG_TO_FILE
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true);
    auto logger = std::make_shared<spdlog::logger>(
        "Logger", spdlog::sinks_init_list{console_sink, file_sink});
#else
    auto logger = std::make_shared<spdlog::logger>("Logger", spdlog::sinks_init_list{console_sink});
#endif
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);
}
