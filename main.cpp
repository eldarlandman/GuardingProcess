using namespace std;
#include <iostream>
#include <thread>
#include <windows.h>
#include <tlhelp32.h>
#include <unistd.h>
#include "funcsDeclerations.h"


DWORD run(LPVOID args){
    char* procName=(char*)args;
    //char* fullPath=findRunningProcess(procName);
    while (isAlive){
        int i=4;
        while (findRunningProcess(procName)){
            sleep(1);
        }
        reOpenProc(procName);
    }

    //
}


void reOpenProc(char* procName) {
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // start the program up
    CreateProcess( NULL,   // the path
                   procName,
                   NULL,           // Process handle not inheritable
                   NULL,           // Thread handle not inheritable
                   FALSE,          // Set handle inheritance to FALSE
                   0,              // No creation flags
                   NULL,           // Use parent's environment block
                   NULL,           // Use parent's starting directory
                   &si,            // Pointer to STARTUPINFO structure
                   &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

char* findRunningProcess(string process) {
/*
Function takes in a string value for the process it is looking for like ST3Monitor.exe
then loops through all of the processes that are currently running on windows.
If the process is found it is running, therefore the function returns true.
*/
    string compare;
   // bool procRunning = false;
    char* fullPath=NULL;
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    HANDLE hProcess;
    //MODULEENTRY32 lpme;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        fullPath= NULL;
    } else {
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hProcessSnap,&pe32)) { // Gets first running process
            if (pe32.szExeFile== process) {
                hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, pe32.th32ProcessID);
                DWORD value = MAX_PATH;
                char buffer[MAX_PATH];
                QueryFullProcessImageName(hProcess, 0, buffer, &value);
                fullPath=buffer;
            } else {
                // loop through all running processes looking for process
                while (Process32Next(hProcessSnap,&pe32)) {
                    // Set to an AnsiString instead of Char[] to make compare easier
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

    return fullPath;//***********
}

///////////////////////////////////////////////////////////
//TODO: change all functions to start with capital letter;
//TODO: Add refferences;
int main(int argc, char* argv[]) {

    int isAlive=1; //Changes to zero when user type 'stop'
    string input; //User input
    DWORD pid;

    if (argc <= 1) {
        cout << "invalid number of arguments";
    }
    else{
        char* procName=argv[1];

        if (findRunningProcess(procName)){
           CreateThread( NULL,                   // default security attributes
                         0,                      // use default stack size
                         run,       // thread function name
                         procName,          // argument to thread function
                         0,                      // use default creation flags
                         &pid);
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
            cout<<"Process "<<procName<<" is not a running process!"<<endl;
            cout<<"exiting...";
        }

    }



}

