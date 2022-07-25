#include "../RujinPCH.h"

#include "Logger.h"
#include "General.h"
#include "Macros.h"
#include "ConsoleColors.h"

#include <fcntl.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

Logger::ConsoleLogger* Logger::m_ConsoleLogger = nullptr;
Logger::FileLogger* Logger::m_FileLogger = nullptr;
bool Logger::m_AppendTimestamp = false;

std::map<LogLevel, std::string> Logger::m_LevelToStr = {
	{LogLevel::Debug, "DEBUG"},
	{LogLevel::Info, "INFO"},
	{LogLevel::Warning, "WARNING"},
	{LogLevel::Error, "ERROR"}
};

std::map<LogLevel, std::string> Logger::m_LevelToConsoleStyle = {
	{LogLevel::Debug, KCYN},
	{LogLevel::Info, KWHT},
	{LogLevel::Warning, KYEL},
	{LogLevel::Error, KRED}
};

void Logger::BaseLogger::Log(const std::string& message, bool appendTimestamp)
{
	if (appendTimestamp)
	{
		tm now = rutils::GetDisplayTime();
		(*m_os) << "[" << now.tm_year << "-" << now.tm_mon << "-" << now.tm_mday << " - ";
		(*m_os) << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << ":" << "] ";
	}

	(*m_os) << message;
	m_os->flush();
}

Logger::FileLogger::FileLogger(const std::string& fileName)
	: m_filename(fileName)
{
	m_os = new std::ofstream(m_filename.c_str());
}

Logger::FileLogger::~FileLogger()
{
	if (m_os)
	{
		auto* of = static_cast<std::ofstream*>(m_os);
		of->close();
		delete m_os;
	}
}

Logger::ConsoleLogger::ConsoleLogger()
{
	m_os = &std::cout;
}

void Logger::Initialize()
{
	m_ConsoleLogger = new ConsoleLogger();
}

void Logger::Release()
{
	SAFE_DELETE(m_ConsoleLogger);
	SAFE_DELETE(m_FileLogger);
}

void Logger::ClearConsole()
{
	std::system("cls");
}

void Logger::StartFileLogging(const std::string& fileName)
{
	SAFE_DELETE(m_FileLogger);

	m_FileLogger = new FileLogger(fileName);
}

void Logger::StopFileLogging()
{
	SAFE_DELETE(m_FileLogger);
}

bool Logger::ProcessLog(LogLevel level, const LogString& fmt, fmt::format_args args)
{
	//Skip Debug/T0d0 message in release build
#ifdef NDEBUG
	if (level == LogLevel::Debug) return false;
#endif

	//Generate Message
	std::string logMsg = fmt.format ? fmt::vformat(fmt.message, args) : fmt.message; //DEFAULT FORMATTING

	const auto filename = fs::path(fmt.file).filename().string();
	const std::string strLevel = m_LevelToStr[level];
	const auto colorCode = m_LevelToConsoleStyle[level];

	const auto full_log = fmt::format("[{}{}{}] > {} (line {}) :: {}\n", colorCode, strLevel, RST, filename, fmt.line, logMsg);

	//Console Log
	if (m_ConsoleLogger)
	{
		m_ConsoleLogger->Log(full_log);
	}

	//File Log
	if (m_FileLogger)
	{
		m_FileLogger->Log(full_log, true);
	}

	//Show MessageBox
	if (level == LogLevel::Error)
	{
		throw std::runtime_error(full_log);
	}

	return true;
}