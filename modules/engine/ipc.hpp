#pragma once
#include <windows.h>
#include <iostream>
#include <thread>

namespace owo
{
    /**
     * @brief Parent class, contains the [func] callback template
     */
    class IPCCall
    {
    public:
        virtual void func(char* data, unsigned short length) = 0;
    };


    /**
     * @brief Contains the type class method and instance 
     * @tparam T Type of the stored class
     */
    template<class T>
    class TypedIPCCall: public virtual IPCCall
    {
    private:
        void (T::*callback)(char* data, unsigned short length);
        T* instance;
    public:
        TypedIPCCall(void (T::*__callback)(char* data, unsigned short length), T* __instance)
        {
            this->callback = __callback;
            this->instance = __instance;
        }

        void func(char* data, unsigned short length)
        {
            (this->instance->*this->callback)(data, length);
        }
    };

    class IPC
    {
    private:
        HANDLE g_hChildStd_IN_Rd = NULL;
        HANDLE g_hChildStd_IN_Wr = NULL;
        HANDLE g_hChildStd_OUT_Rd = NULL;
        HANDLE g_hChildStd_OUT_Wr = NULL;
        SECURITY_ATTRIBUTES saAttr;

        IPCCall* call;
        std::thread readingThread;
        bool running;

        void init()
        {
            saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
            saAttr.bInheritHandle = TRUE; 
            saAttr.lpSecurityDescriptor = NULL; 
            
            // Create the pipes for the child process
            if ( ! CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0) ) 
                std::cout << "Error: Error during StdoutRd CreatePipe" << std::endl;
            if ( ! SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) )
                std::cout << "Error: Error during Stdout SetHandleInformation" << std::endl;
            if (! CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) 
                std::cout << "Error: Error during Stdin CreatePipe" << std::endl;
            if ( ! SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
                std::cout << "Error: Error during Stdin SetHandleInformation" << std::endl;
        }

    public:
        IPC()
        {
            init();
        }

        bool startChild()
        {
            // Create the child process
            char* command = (char*) "python ./modules/engine/tracker.py";
            PROCESS_INFORMATION piProcInfo;
            STARTUPINFO siStartInfo;
            BOOL bSuccess = FALSE; 
            
            ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
            ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
            siStartInfo.cb = sizeof(STARTUPINFO); 
            siStartInfo.hStdError = g_hChildStd_OUT_Wr;
            siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
            siStartInfo.hStdInput = g_hChildStd_IN_Rd;
            siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
            
            bSuccess = CreateProcess(NULL, 
                command,       // command line 
                NULL,          // process security attributes 
                NULL,          // primary thread security attributes 
                TRUE,          // handles are inherited 
                0,             // creation flags 
                NULL,          // use parent's environment 
                NULL,          // use parent's current directory 
                &siStartInfo,  // STARTUPINFO pointer 
                &piProcInfo);  // receives PROCESS_INFORMATION 
            
            if ( ! bSuccess ) 
            {
                std::cout << "Error: Error creating the child process" << std::endl;
                return false;
            }
            else 
            {
                CloseHandle(piProcInfo.hProcess);
                CloseHandle(piProcInfo.hThread);
                CloseHandle(g_hChildStd_OUT_Wr);
                CloseHandle(g_hChildStd_IN_Rd);
            }

            this->running = true;

            //start the reading thread
            this->readingThread = std::thread(&IPC::_read_output, this);
            return true;
        }

        void _read_output()
        {
            DWORD dwRead, dwWritten; 
            const int SIZE = 1024;
            char chBuf[SIZE];
            BOOL bSuccess = FALSE;

            for (;;) 
            { 
                bSuccess = ReadFile( g_hChildStd_OUT_Rd, chBuf, SIZE, &dwRead, NULL);
                if (this->call != nullptr && dwRead > 0)
                    this->call->func(chBuf, dwRead);
                if( ! bSuccess || dwRead == 0 || !this->running)
                    break;
            }
        }

        template<class T> void setReadCallback(void (T::*callback)(char* data, unsigned short length), T *c)
        {
            this->call = new TypedIPCCall<T>(callback, c);
        }

        void writeToChild(const char* data, unsigned int length)
        {
            if (!running) return;
            DWORD dwWritten;
            WriteFile(g_hChildStd_IN_Wr, (LPCVOID)data, length, &dwWritten, 0);
        }

        void writeToChild(unsigned char* data, unsigned int length)
        {
            if (!running) return;
            DWORD dwWritten;
            WriteFile(g_hChildStd_IN_Wr, (LPCVOID)data, length, &dwWritten, 0);
        }

        void stopChild()
        {
            writeToChild("STOP\n", 6);
            this->running = false;
            this->readingThread.join();
        }

        ~IPC()
        {
            if (running)
            {
                this->running = false;
                this->readingThread.join();
            }
        }
    };
}