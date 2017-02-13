#pragma once

namespace ODiagnostics
{
    enum Severity {LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR};

    void LogError(wchar_t* fmt, ...);
    void LogWarning(wchar_t* fmt, ...);
    void LogInfo(wchar_t* fmt, ...);
    void LogDebug(wchar_t* fmt, ...);

    void LogMessage(Severity s, wchar_t* fmt, va_list args);

    class Logger
    {
    public:
        static Logger& GetInstance();
        void Log(Severity s, wchar_t* fmt, va_list args);
        void Flush();
        void SetSeverity(Severity s);
    private:
        Logger();
        ~Logger();

        void NewFile();
        void Rotate();
        wchar_t* SeverityToString(Severity s);

        Severity sev_;
        FILE* file_;
        SYSTEMTIME logDate_;
        size_t timeLength_;
        bool ignoreSeverity_;
    };
}
