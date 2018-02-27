using namespace std;
#include <iostream>
#include <thread>
#include <windows.h>
#include <tlhelp32.h>
#include <unistd.h>
#include <tchar.h>
#include "funcsDeclerations.h"




DWORD Run(LPVOID args){
    char* procName=(char*)args;
    //char* fullPath=findRunningProcess(procName);  //Actually never used. Works fine with the process executable name.
    while (isAlive){
        int i=4;
        while (FindRunningProcess(procName)){
            sleep(1);
        }
        ReOpenProc(procName);
    }

}

void ReOpenProc(char* procName) {
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    // set the size of the structures
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

//    BOOL WINAPI CreateProcess(
//            _In_opt_    LPCTSTR               lpApplicationName,
//            _Inout_opt_ LPTSTR                lpCommandLine,
//            _In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
//            _In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
//            _In_        BOOL                  bInheritHandles,
//            _In_        DWORD                 dwCreationFlags,
//            _In_opt_    LPVOID                lpEnvironment,
//            _In_opt_    LPCTSTR               lpCurrentDirectory,
//            _In_        LPSTARTUPINFO         lpStartupInfo,
//            _Out_       LPPROCESS_INFORMATION lpProcessInformation
//    );

    // start the program up
    CreateProcess( NULL,   // lpApplicationName- The name of the module to be executed.
                           // The string can specify the full path and file name of the module to execute
                           // or it can specify a partial name.
                           //The lpApplicationName parameter can be NULL.
                           // In that case, the module name must be the first white space–delimited token in the lpCommandLine string.

                   procName, //The command line to be executed.
                             //this parameter cannot be a pointer to read-only memory (such as a const variable or a literal string).
                             // If this parameter is a constant string, the function may cause an access violation.
                             //system searches for the execuable file in the following sequence:
                               //The directory from which the application loaded.
                               // The current directory for the parent process.
                               // The 32-bit Windows system directory. Use the GetSystemDirectory function to get the path of this directory.
                               //The 16-bit Windows system directory. There is no function that obtains the path of this directory, but it is searched. The name of this directory is System.
                               //The Windows directory. Use the GetWindowsDirectory function to get the path of this directory.
                               //The directories that are listed in the PATH environment variable. Note that this function does not search the per-application path specified by the App Paths registry key. To include this per-application path in the search sequence, use the ShellExecute function.

                   NULL,           // Process handle not inheritable.
                                   //A pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned handle
                                   // to the new thread object can be inherited by child processes
                                   // If lpThreadAttributes is NULL, the handle cannot be inherited.
                                   //In more details: Inside the struct there is a member lpSecurityDescriptor- if lpProcessAttributes is NULL so lpSecurityDescriptor is NULL
                   NULL,           // Thread handle not inheritable (same logic as for process handle)

                   FALSE,          // Set handle inheritance to FALSE, means that the handles of the calling process are not inherited.
                   0,              // No creation flags. Defaults to NORMAL_PRIORITY_CLASS.
                                   // Used to determine the scheduling priorities of the process's threads.

                   NULL,           // Use parent's environment block

                   NULL,           // Use parent's starting directory
                   &si,            // Pointer to STARTUPINFO structure
                   &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    // Close process and thread handles.
    cout<< si.lpDesktop << " , " <<  si.lpTitle << " , " << "sizes: width:"<< si.dwXSize<<" , height: "<<si.dwYSize <<" , screen buffer width:"<< si.dwXCountChars << " ,colors:  " <<si.dwFillAttribute<<endl;
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

char* FindRunningProcess(string process) {
/*
Function takes in a string value for the process it is looking for like ST3Monitor.exe
then loops through all of the processes that are currently running on windows.
If the process is found it is running , therefore the function returns his path. Else- returns NULL
*/
    string compare;
    char* fullPath=NULL;
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    HANDLE hProcess;

    //Takes a snapshot of the specified processes, as well as the heaps, modules, and threads used by these processes.
    //TH32CS_SNAPPROCESS-Includes all processes in the system in the snapshot. Other flags can includes all modules/threads etc.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        fullPath= NULL;
    } else {
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hProcessSnap,&pe32)) { // Gets first running process
            printProcDetails(&pe32);
            if (pe32.szExeFile== process) { //the next lines retrieve the full path of the process
                //PROCESS_QUERY_INFORMATION - Required to retrieve certain information about a process, such as its token, exit code, and priority class
                //false- If this value is TRUE, processes created by this process will inherit the handle. Otherwise, the processes do not inherit this handle.
                //th32processId- The identifier of the local process to be opened.
                hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, pe32.th32ProcessID);
                DWORD value = MAX_PATH;
                char buffer[MAX_PATH];
                //QueryFullProcessImageName- take HANDLE to hProcess and copy his name to buffer.
                // '0' for format name (Win32) and value for the size.
                QueryFullProcessImageName(hProcess, 0, buffer, &value);
                fullPath=buffer;
            } else {
                // Loop through all running processes looking for process
                while (Process32Next(hProcessSnap,&pe32)) {
                    printProcDetails(&pe32);
                    compare = pe32.szExeFile;
                    if (compare == process)  {
                        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, pe32.th32ProcessID);
                        DWORD value = MAX_PATH;
                        char buffer[MAX_PATH];
                        QueryFullProcessImageName(hProcess, 0, buffer, &value);
                        fullPath=buffer;
                        break;
                    }
                }
            }
            // clean the snapshot object
            CloseHandle(hProcessSnap);
        }
    }
    return fullPath;
}

void printProcDetails(PROCESSENTRY32 * pe32) {
    cout<<"Process Name: "<< pe32->szExeFile<<endl;
    cout<<"Process Id: "<< pe32->th32ProcessID<<endl;
    cout<<"Thread count: "<< pe32->cntThreads<<endl;
    cout<<"Parent process ID"<< pe32->th32ParentProcessID<<endl;
    cout<<"Pariority base: "<< pe32->pcPriClassBase<<endl;

//    cout<<"List of Modules:"<<endl;
//    ListProcessModules(pe32->th32ParentProcessID);
//    cout<<endl;
//    cout<<"- - - - - - - - - - -"<<endl;
//    cout<<"List of threads:"<<endl;
//    ListProcessThreads(pe32->th32ParentProcessID);
//    cout<<"++++++++++++++++++++"<<endl;
//    cout<<endl<<endl;
}

BOOL ListProcessModules( DWORD dwPID )
{
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32;

    // Take a snapshot of all modules in the specified process.
    hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
    if( hModuleSnap == INVALID_HANDLE_VALUE )
    {
        printError( TEXT("CreateToolhelp32Snapshot (of modules)") );
        return( FALSE );
    }

    // Set the size of the structure before using it.
    me32.dwSize = sizeof( MODULEENTRY32 );

    // Retrieve information about the first module,
    // and exit if unsuccessful
    if( !Module32First( hModuleSnap, &me32 ) )
    {
        printError( TEXT("Module32First") );  // show cause of failure
        CloseHandle( hModuleSnap );           // clean the snapshot object
        return( FALSE );
    }

    // Now walk the module list of the process,
    // and display information about each module
    do
    {
        printf( TEXT("\n\n     MODULE NAME:     %s"),   me32.szModule );
        printf( TEXT("\n     Executable     = %s"),     me32.szExePath );
        printf( TEXT("\n     Process ID     = 0x%08X"),         me32.th32ProcessID );
        printf( TEXT("\n     Ref count (g)  = 0x%04X"),     me32.GlblcntUsage );
        printf( TEXT("\n     Ref count (p)  = 0x%04X"),     me32.ProccntUsage );
        printf( TEXT("\n     Base address   = 0x%08X"),  me32.modBaseAddr );
        printf( TEXT("\n     Base size      = %d"),             me32.modBaseSize );

    } while( Module32Next( hModuleSnap, &me32 ) );

    CloseHandle( hModuleSnap );
    return( TRUE );
}

BOOL ListProcessThreads( DWORD dwOwnerPID )
{
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32 te32;

    // Take a snapshot of all running threads
    hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );
    if( hThreadSnap == INVALID_HANDLE_VALUE )
        return( FALSE );

    // Fill in the size of the structure before using it.
    te32.dwSize = sizeof(THREADENTRY32);

    // Retrieve information about the first thread,
    // and exit if unsuccessful
    if( !Thread32First( hThreadSnap, &te32 ) )
    {
        printError( TEXT("Thread32First") ); // show cause of failure
        CloseHandle( hThreadSnap );          // clean the snapshot object
        return( FALSE );
    }

    // Now walk the thread list of the system,
    // and display information about each thread
    // associated with the specified process
    do
    {
        if( te32.th32OwnerProcessID == dwOwnerPID )
        {
            printf( TEXT("\n\n     THREAD ID      = 0x%08X"), te32.th32ThreadID );
            printf( TEXT("\n     Base priority  = %d"), te32.tpBasePri );
            printf( TEXT("\n     Delta priority = %d"), te32.tpDeltaPri );
            printf( TEXT("\n"));
        }
    } while( Thread32Next(hThreadSnap, &te32 ) );

    CloseHandle( hThreadSnap );
    return( TRUE );
}

void printError( TCHAR* msg )
{
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;

    eNum = GetLastError( );
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, eNum,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                   sysMsg, 256, NULL );

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while( ( *p > 31 ) || ( *p == 9 ) )
        ++p;
    do { *p-- = 0; } while( ( p >= sysMsg ) &&
                            ( ( *p == '.' ) || ( *p < 33 ) ) );

    // Display the message
    _tprintf( TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg );
}

///////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {

    int isAlive=1; //Changes to zero when user type 'stop'
    string input; //User input
    DWORD pid;

    if (argc <= 1) {
        cout << "invalid number of arguments";
    }
    else{
        char* procName=argv[1];

//        HANDLE WINAPI CreateThread(
//                _In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,  //
//                _In_      SIZE_T                 dwStackSize,
//                _In_      LPTHREAD_START_ROUTINE lpStartAddress,
//                _In_opt_  LPVOID                 lpParameter,
//                _In_      DWORD                  dwCreationFlags,
//                _Out_opt_ LPDWORD                lpThreadId
//        );
        if (FindRunningProcess(procName)){
           CreateThread( NULL,                   // default security attributes-the returned handle  cannot be inherited
                                                 // If the value of this member is NULL,
                                                 // the object is assigned the default security descriptor associated with the access token of the calling process.
                                                 //An access token contains the security information for a logon session.
                                                 // The system creates an access token when a user logs on,
                                                 // and every process executed on behalf of the user has a copy of the token.

                         0,                      // use default stack size.
                                                 // It is best to choose as small a stack size as possible and commit the stack that is needed for the thread or fiber to run reliably.
                                                 // Every page that is reserved for the stack cannot be used for any other purpose.
                                                 //A stack is freed when its thread exits. It is not freed if the thread is terminated by another thread.

                         Run,                    // thread function name

                         procName,               // argument to thread function

                         0,                      // use default creation flags
                                                 // 0 means- thread runs immediately after creation/thread created in suspended state, etc.)
                         &pid); //thread identifier
            cout<<"Thread "<<pid<<" start guarding process "<<procName<<"..."<<endl;
            cout<<"Type 'stop' to exit:\n";
            while(cin>>input){

                if (input=="stop"){
                    isAlive=false;
                    cout<<"Stop guarding process..."<<endl;
                    cout<<"bye-bye!";

                    break;
                }
                else{
                    cout<<"Wrong input!\n";
                    cout<<"Type 'stop' to exit:\n";
                }
            }
        }
        else{
            cout<<"Process "<<procName<<" was not found"<<endl;
            cout<<"exiting...";
        }

    }



}

