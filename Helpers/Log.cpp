#include "StdAfx.h"
#include "Log.h"

using namespace OHelpers;

namespace ODiagnostics
{
    Logger& Logger::GetInstance()
    {
        static Logger instance;
        return instance;
    }

    void Logger::Log(Severity s, wchar_t* fmt, va_list args)
    {
        if (!ignoreSeverity_ && s < sev_) {
            return;
        }
        Rotate();
        SYSTEMTIME timeNow;
        GetSystemTime(&timeNow);
        wchar_t logLine[1024] = { 0 };
        swprintf_s(logLine, L"%.2d:%.2d:%.2d.%.3d - %ls:  ", timeNow.wHour, timeNow.wMinute, timeNow.wSecond, timeNow.wMilliseconds, SeverityToString(s));
        if (timeLength_ == 0) {
            timeLength_ = wcslen(logLine);
        }
        wchar_t* logLineItr = logLine + timeLength_;

        vswprintf_s(logLineItr, 1024 - timeLength_, fmt, args);
        
        fwprintf_s(file_, L"%ls\n", logLine);
#ifdef DEBUG
        std::wcout << logLine << std::endl;
#endif

    }

    Logger::Logger() noexcept :
        sev_(LOG_INFO),
        file_(nullptr),
        timeLength_(0),
        ignoreSeverity_(false)
    {
        
    }

    Logger::~Logger()
    {
        fwprintf_s(file_, L"\n\n");
        if (file_) {
            fclose(file_);
            file_ = nullptr;
        }
    }

    void Logger::NewFile()
    {
        if (file_) {
            CloseHandle(file_);
        }
        GetSystemTime(&logDate_);
        wchar_t path[O_MAX_PATH] = { 0 };
        if (!DirectoryExists(L"Logs")) {
            CreateDirectory(L"Logs", 0);
        }
        swprintf_s(path, L"Logs\\Oklahoma_%.2d_%.2d_%.2d.log", logDate_.wDay, logDate_.wMonth, logDate_.wYear%100);
        file_ = _wfsopen(path, L"a+", _SH_DENYNO);
    }

    void Logger::Rotate()
    {
        if (!file_) {
            NewFile();
            return;
        }
        SYSTEMTIME dateNow;
        GetSystemTime(&dateNow);
        if (dateNow.wDay != logDate_.wDay || dateNow.wMonth != logDate_.wMonth || dateNow.wYear != logDate_.wYear) {
            NewFile();
        }
    }

    wchar_t* Logger::SeverityToString(Severity s)
    {
        switch (s)
        {
        case ODiagnostics::LOG_DEBUG:
            return L"Debug";
        case ODiagnostics::LOG_INFO:
            return L"Info";
        case ODiagnostics::LOG_WARNING:
            return L"Warn";
        case ODiagnostics::LOG_ERROR:
            return L"Error";
        default:
            assert(0);
        }
        return nullptr;
    }

    void Logger::Flush()
    {
        if (file_) {
            fflush(file_);
        }
    }

    void Logger::SetSeverity(Severity s)
    {
        sev_ = s;
        ignoreSeverity_ = true;
        LogInfo(L"Log severity set to: %ls", SeverityToString(s));
        ignoreSeverity_ = false;
    }

    void LogError(wchar_t* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        LogMessage(LOG_ERROR, fmt, args);
        va_end(args);
    }

    void LogWarning(wchar_t* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        LogMessage(LOG_WARNING, fmt, args);
        va_end(args);
    }

    void LogInfo(wchar_t* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        LogMessage(LOG_INFO, fmt, args);
        va_end(args);
    }

    void LogDebug(wchar_t* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        LogMessage(LOG_DEBUG, fmt, args);
        va_end(args);
    }

    void LogMessage(Severity s, wchar_t* fmt, va_list args)
    {
        Logger::GetInstance().Log(s, fmt, args);
        Logger::GetInstance().Flush();
    }

}
