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
        if (s < sev_) {
            return;
        }
        Rotate();
        SYSTEMTIME timeNow;
        GetSystemTime(&timeNow);
        wchar_t logLine[1024];
        wchar_t timeFormat[] = L"%.2d:%.2d:%.2d.%.3d: ";
        swprintf_s(logLine, timeFormat, timeNow.wHour, timeNow.wMinute, timeNow.wSecond, timeNow.wMilliseconds);
        if (timeLength_ == 0) {
            timeLength_ = wcslen(logLine);
        }
        wchar_t* logLineItr = logLine + timeLength_;

        vswprintf_s(logLineItr, 1024 - timeLength_, fmt, args);
        
        fwprintf_s(file_, L"%ls\n", logLine);
    }

    Logger::Logger():
        sev_(LOG_INFO),
        file_(nullptr),
        timeLength_(0)
    {
        
    }

    Logger::~Logger()
    {
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
        wchar_t path[O_MAX_PATH];
        if (!DirectoryExists(L"Logs")) {
            CreateDirectory(L"Logs", 0);
        }
        swprintf_s(path, L"Logs\\Oklahoma_%2d_%2d_%2d.log", logDate_.wDay, logDate_.wMonth, logDate_.wYear);
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

    void Logger::Flush()
    {
        if (file_) {
            fflush(file_);
        }
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
