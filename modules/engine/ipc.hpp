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
    /**
     * @brief The callback function
     * 
     * @param data The data to send
     * @param length The length of the data
     */
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

    /**
     * @brief IPC class, takes care of IPC communication between the child processes (Python) and this process (C++)
     */
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

        /**
         * @brief Initialises the IPC's attributes
         */
        void init()
        {
            saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
            saAttr.bInheritHandle = TRUE; 
            saAttr.lpSecurityDescriptor = NULL;
        }

    public:
        /**
         * @brief Default constructor
         */
        IPC()
        {
            init();
        }

        /**
         * @brief Starts the child process (Python)
         * @return If the process as been successfully started or not
         */
        bool startChild()
        {
            // Create the pipes for the child process
            if ( ! CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0) ) 
                std::cout << "Error: Error during StdoutRd CreatePipe" << std::endl;
            if ( ! SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) )
                std::cout << "Error: Error during Stdout SetHandleInformation" << std::endl;
            if (! CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) 
                std::cout << "Error: Error during Stdin CreatePipe" << std::endl;
            if ( ! SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
                std::cout << "Error: Error during Stdin SetHandleInformation" << std::endl;
                
            // Create the child process
            char* command = (char*) "./modules/engine/tracker/tracker.exe";
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
                &piProcInfo    // receives PROCESS_INFORMATION 
            );
            
            if ( bSuccess )
            {
                CloseHandle(piProcInfo.hProcess);
                CloseHandle(piProcInfo.hThread);
                CloseHandle(g_hChildStd_OUT_Wr);
                CloseHandle(g_hChildStd_IN_Rd);
            }
            else return false;

            //start the reading thread
            this->readingThread = std::thread(&IPC::_read_output, this);
            this->running = true;
            return true;
        }

        /**
         * @brief [INTERNAL] Reads the child's stdout and calls the read callback
         * if new data is available
         */
        void _read_output()
        {
            DWORD dwRead, dwWritten; 
            const int SIZE = 1024;
            char chBuf[SIZE];
            BOOL bSuccess = FALSE;

            while (this->running)
            { 
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                bSuccess = ReadFile( g_hChildStd_OUT_Rd, chBuf, SIZE, &dwRead, NULL);
                if( ! bSuccess || dwRead == 0)
                    break;
                if (this->call != nullptr && dwRead > 0)
                    this->call->func(chBuf, dwRead);
            }
        }

        /**
         * @brief Sets the read callback method, called when new data from the child's output is detected
         * @tparam T The method's class
         * @param callback The read callback
         * @param c The class instance
         */
        template<class T> void setReadCallback(void (T::*callback)(char* data, unsigned short length), T *c)
        {
            this->call = new TypedIPCCall<T>(callback, c);
        }

        /**
         * @brief Writes to the child's stdin the given data
         * 
         * @param data Data to write to the child
         * @param length Length of the data to write
         */
        void writeToChild(const char* data, unsigned int length)
        {
            if (!running) return;
            DWORD dwWritten;
            WriteFile(g_hChildStd_IN_Wr, (LPCVOID)data, length, &dwWritten, 0);
        }

        /**
         * @brief Writes to the child's stdin the given data
         * 
         * @param data Data to write to the child
         * @param length Length of the data to write
         */
        void writeToChild(unsigned char* data, unsigned int length)
        {
            if (!running) return;
            DWORD dwWritten;
            WriteFile(g_hChildStd_IN_Wr, (LPCVOID)data, length, &dwWritten, 0);
        }

        /**
         * @brief Sends a STOP signal to the child process to stop it
         * and stops the child's stdout reading thread
         */
        void stopChild()
        {
            if (!this->running) return;
            writeToChild("STOP\n", 6);
            this->running = false;
            this->readingThread.join();
        }

        ~IPC()
        {
            this->stopChild();
        }
    };
}