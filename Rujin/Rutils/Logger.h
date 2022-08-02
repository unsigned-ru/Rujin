#ifndef LOGGER_H
#define LOGGER_H

#include <map>

enum class LogLevel : uint32_t
{
	Debug = 0x1,
	Info = 0x2,
	Warning = 0x4,
	Error = 0x8,
};

#pragma region LoggingMacros
#define LOG_DEBUG(msg)  Logger::LogDebug(Logger::LogString{msg, __FILE__, __LINE__, false})
#define LOG_DEBUG_(fmt, ...)  Logger::LogDebug(Logger::LogString{fmt, __FILE__, __LINE__, true}, __VA_ARGS__)

#define LOG_INFO(msg)  Logger::LogInfo(Logger::LogString{msg, __FILE__, __LINE__, false})
#define LOG_INFO_(fmt, ...)  Logger::LogInfo(Logger::LogString{fmt, __FILE__, __LINE__, true}, __VA_ARGS__)

#define LOG_WARNING(msg)  Logger::LogWarning(Logger::LogString{msg, __FILE__, __LINE__, false})
#define LOG_WARNING_(fmt, ...)  Logger::LogWarning(Logger::LogString{fmt, __FILE__, __LINE__, true}, __VA_ARGS__)

#define LOG_ERROR(msg)  Logger::LogError(Logger::LogString{msg, __FILE__, __LINE__, false})
#define LOG_ERROR_(fmt, ...)  Logger::LogError(Logger::LogString{fmt, __FILE__, __LINE__, true}, __VA_ARGS__)
#pragma endregion

class Logger
{
	class BaseLogger
	{
	protected:
		std::ostream* m_os = nullptr;
	public:
		BaseLogger() = default;
		virtual ~BaseLogger() = default;

		virtual void Log(const std::string& message, bool appendTimestamp = false);
	};

	class FileLogger final : public BaseLogger
	{
		std::string m_filename;
	public:
		FileLogger(const FileLogger& other) = delete;
		FileLogger(FileLogger&& other) noexcept = delete;
		FileLogger& operator=(const FileLogger& other) = delete;
		FileLogger& operator=(FileLogger&& other) noexcept = delete;

		explicit FileLogger(const std::string& fileName);

		~FileLogger() override;
	};

	class ConsoleLogger final : public BaseLogger
	{
	public:
		ConsoleLogger();
	};

public:
	struct LogString
	{
		std::string message{};
		std::string file{};
		long line{};
		bool format{false};
	};

	~Logger() = default;
	Logger(const Logger& other) = delete;
	Logger(Logger&& other) noexcept = delete;
	Logger& operator=(const Logger& other) = delete;
	Logger& operator=(Logger&& other) noexcept = delete;

	static void Initialize();
	static void Release();

	template <typename ... Args>
	static void LogDebug(const LogString& fmt, Args&&... args);

	template <typename ... Args>
	static void LogWarning(const LogString& fmt, Args&&... args);

	template <typename ... Args>
	static void LogInfo(const LogString& fmt, Args&&... args);

	template <typename ... Args>
	static bool LogError(const LogString& fmt, Args&&... args);

	static void ClearConsole();
	static void AppendTimestamp(bool append) { m_AppendTimestamp = append; }
	static void StartFileLogging(const std::string& fileName);
	static void StopFileLogging();

private:
	Logger() = default;

	static bool ProcessLog(LogLevel level, const LogString& fmt, fmt::format_args args);

	static bool m_AppendTimestamp;

	static ConsoleLogger* m_ConsoleLogger;
	static FileLogger* m_FileLogger;

	static std::map<LogLevel, std::string> m_LevelToStr;
	static std::map<LogLevel, std::string> m_LevelToConsoleStyle;
};

template <typename ... Args>
void Logger::LogInfo(const LogString& fmt, Args&&... args)
{
	ProcessLog(LogLevel::Info, fmt, fmt::make_format_args(args...));
}

template <typename ... Args>
void Logger::LogDebug(const LogString& fmt, Args&&... args)
{
	ProcessLog(LogLevel::Debug, fmt, fmt::make_format_args(args...));
}

template <typename ... Args>
void Logger::LogWarning(const LogString& fmt, Args&&... args)
{
	ProcessLog(LogLevel::Warning, fmt, fmt::make_format_args(args...));
}

template <typename ... Args>
bool Logger::LogError(const LogString& fmt, Args&&... args)
{
	return ProcessLog(LogLevel::Error, fmt, fmt::make_format_args(args...));
}

#endif // Include Guard: LOGGER_H
