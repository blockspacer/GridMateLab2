/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#pragma once

#include <ILog.h>
#include <CryThread.h>
#include <MultiThread.h>
#include <MultiThread_Containers.h>

//////////////////////////////////////////////////////////////////////
#if defined(ANDROID) || defined(AZ_PLATFORM_APPLE_OSX)
    #define MAX_TEMP_LENGTH_SIZE    4098
#define AZ_RESTRICTED_SECTION_IMPLEMENTED
#elif defined(AZ_RESTRICTED_PLATFORM)
#include AZ_RESTRICTED_FILE(Log_h, AZ_RESTRICTED_PLATFORM)
#endif
#if defined(AZ_RESTRICTED_SECTION_IMPLEMENTED)
#undef AZ_RESTRICTED_SECTION_IMPLEMENTED
#else
    #define MAX_TEMP_LENGTH_SIZE    8196
#endif
#define MAX_FILENAME_SIZE           256

#define KEEP_LOG_FILE_OPEN


//////////////////////////////////////////////////////////////////////
class CLog
    : public ILog
{
public:
    typedef std::list<ILogCallback*> Callbacks;
    typedef CryStackStringT<char, MAX_TEMP_LENGTH_SIZE> LogStringType;

    // constructor
    CLog(ISystem* pSystem);
    // destructor
    ~CLog();


    // interface ILog, IMiniLog -------------------------------------------------

    virtual void Release() { delete this; };
    virtual bool SetFileName(const char* fileNameOrAbsolutePath, bool backupLogs);
    virtual const char* GetFileName();
    virtual const char* GetBackupFileName();
#if !defined(EXCLUDE_NORMAL_LOG)
    virtual void Log(const char* command, ...) PRINTF_PARAMS(2, 3);
    virtual void LogAlways(const char* command, ...) PRINTF_PARAMS(2, 3);
    virtual void LogWarning(const char* command, ...) PRINTF_PARAMS(2, 3);
    virtual void LogError(const char* command, ...) PRINTF_PARAMS(2, 3);
    virtual void LogPlus(const char* command, ...) PRINTF_PARAMS(2, 3);
    virtual void LogToFile  (const char* command, ...) PRINTF_PARAMS(2, 3);
    virtual void LogToFilePlus(const char* command, ...) PRINTF_PARAMS(2, 3);
    virtual void LogToConsole(const char* command, ...) PRINTF_PARAMS(2, 3);
    virtual void LogToConsolePlus(const char* command, ...) PRINTF_PARAMS(2, 3);
#else
    virtual void Log(const char* command, ...) PRINTF_PARAMS(2, 3) {
    };
    virtual void LogAlways(const char* command, ...) PRINTF_PARAMS(2, 3) {
    };
    virtual void LogWarning(const char* command, ...) PRINTF_PARAMS(2, 3) {
    };
    virtual void LogError(const char* command, ...) PRINTF_PARAMS(2, 3) {
    };
    virtual void LogPlus(const char* command, ...) PRINTF_PARAMS(2, 3) {
    };
    virtual void LogToFile  (const char* command, ...) PRINTF_PARAMS(2, 3) {
    };
    virtual void LogToFilePlus(const char* command, ...) PRINTF_PARAMS(2, 3) {
    };
    virtual void LogToConsole(const char* command, ...) PRINTF_PARAMS(2, 3) {
    };
    virtual void LogToConsolePlus(const char* command, ...) PRINTF_PARAMS(2, 3) {
    };
#endif // !defined(EXCLUDE_NORMAL_LOG)
    virtual void UpdateLoadingScreen(const char* command, ...) PRINTF_PARAMS(2, 3);
    virtual void SetVerbosity(int verbosity);
    virtual int  GetVerbosityLevel();
    virtual void RegisterConsoleVariables();
    virtual void UnregisterConsoleVariables();
    virtual void AddCallback(ILogCallback* pCallback);
    virtual void RemoveCallback(ILogCallback* pCallback);
    virtual void LogV(const ELogType ineType, int flags, const char* szFormat, va_list args);
    virtual void LogV(const ELogType ineType, const char* szFormat, va_list args);
    virtual void Update();
    virtual const char* GetModuleFilter();
    virtual void FlushAndClose();

private: // -------------------------------------------------------------------
    struct SLogMsg
    {
        enum class Destination
        {
            Default, //LogString, sends OnWrite to anycallback registered with AddCallback
            Console,
            File
        };
        char msg[512];
        ELogType logType;
        bool bAdd;
        Destination destination;
        void GetMemoryUsage(ICrySizer* pSizer) const {}
    };

    bool IsError(ELogType logType) const { return logType == ELogType::eError || logType == ELogType::eErrorAlways || logType == ELogType::eWarning || logType == ELogType::eWarningAlways; }

    //helper function to pass calls to LogString... to the main thread, returns false if you are on the main thread already, in which case just process the work.
    bool LogToMainThread(const char* szString, ELogType logType, bool bAdd, SLogMsg::Destination destination);

    enum class MessageQueueState
    {
        NotQueued,
        Queued
    };

#if !defined(EXCLUDE_NORMAL_LOG)
    void LogString(const char* szString, ELogType logType);
    void LogStringToFile(const char* szString, ELogType logType, bool bAdd, MessageQueueState queueState);
    void LogStringToConsole(const char* szString, ELogType logType, bool bAdd);
#else
    void LogString(const char* szString, ELogType logType) {}
    void LogStringToFile(const char* szString, ELogType logType, bool bAdd, MessageQueueState queueState) {}
    void LogStringToConsole(const char* szString, ELogType logType, bool bAdd) {}
#endif // !defined(EXCLUDE_NORMAL_LOG)

    AZ::IO::HandleType OpenLogFile(const char* filename, const char* mode);
    void CloseLogFile(bool force = false);

    // will format the message into m_szTemp
    void FormatMessage(const char* szCommand, ...) PRINTF_PARAMS(2, 3);

#if defined(SUPPORT_LOG_IDENTER)
    void Indent(CLogIndenter* indenter);
    void Unindent(CLogIndenter* indenter);
    void BuildIndentString();
    virtual void PushAssetScopeName(const char* sAssetType, const char* sName);
    virtual void PopAssetScopeName();
    virtual const char* GetAssetScopeString();
#endif

    ISystem*               m_pSystem;                                                       //
    float                       m_fLastLoadingUpdateTime;                           // for non-frequent streamingEngine update
    //char                      m_szTemp[MAX_TEMP_LENGTH_SIZE];             //
    char                        m_szFilename[MAX_FILENAME_SIZE];            // can be with path
    mutable char        m_sBackupFilename[MAX_FILENAME_SIZE];   // can be with path
    AZ::IO::HandleType m_logFileHandle;
    CryStackStringT<char, 32> m_LogMode;                                                        //mode m_pLogFile has been opened with
    AZ::IO::HandleType m_errFileHandle;
    int                         m_nErrCount;

    bool m_backupLogs;

#if defined(SUPPORT_LOG_IDENTER)
    uint8                   m_indentation;
    LogStringType           m_indentWithString;
    class CLogIndenter* m_topIndenter;

    struct SAssetScopeInfo
    {
        string sType;
        string sName;
    };

    std::vector<SAssetScopeInfo> m_assetScopeQueue;
    CryCriticalSection m_assetScopeQueueLock;
    string m_assetScopeString;
#endif

    ICVar*                 m_pLogIncludeTime;                                       //

    IConsole*          m_pConsole;                                                      //

    CryCriticalSection m_logCriticalSection;

    struct SLogHistoryItem
    {
        char str[MAX_WARNING_LENGTH];
        const char* ptr;
        ELogType type;
        float time;
    };
    SLogHistoryItem m_history[16];
    int m_iLastHistoryItem;

    static bool CheckLogFormatter(const char* formatter);

#if defined(KEEP_LOG_FILE_OPEN)
    static void LogFlushFile(IConsoleCmdArgs* pArgs);

    bool m_bFirstLine;
#endif

public: // -------------------------------------------------------------------

    void GetMemoryUsage(ICrySizer* pSizer) const
    {
        pSizer->AddObject(this, sizeof(*this));
        pSizer->AddObject(m_pLogVerbosity);
        pSizer->AddObject(m_pLogWriteToFile);
        pSizer->AddObject(m_pLogWriteToFileVerbosity);
        pSizer->AddObject(m_pLogVerbosityOverridesWriteToFile);
        pSizer->AddObject(m_pLogSpamDelay);
        pSizer->AddObject(m_threadSafeMsgQueue);
    }
    // checks the verbosity of the message and returns NULL if the message must NOT be
    // logged, or the pointer to the part of the message that should be logged
    const char* CheckAgainstVerbosity(const char* pText, bool& logtofile, bool& logtoconsole, const uint8 DefaultVerbosity = 2);

    // create backup of log file, useful behavior - only on development platform
    void CreateBackupFile() const;

    ICVar*                 m_pLogVerbosity;                                             //
    ICVar*                 m_pLogWriteToFile;                                       //
    ICVar*                 m_pLogWriteToFileVerbosity;                      //
    ICVar*                 m_pLogVerbosityOverridesWriteToFile;     //
    ICVar*                 m_pLogSpamDelay;                       //
    ICVar*                 m_pLogModule;                                                    // Module filter for log
    Callbacks               m_callbacks;                                                    //

    threadID m_nMainThreadId;
    CryMT::queue<SLogMsg> m_threadSafeMsgQueue;
};
