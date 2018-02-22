using namespace std;
#include <iostream>
#include <thread>
#include <windows.h>
#include <tlhelp32.h>
#include <unistd.h>
#include "funcsDeclerations.h"


DWORD run(LPVOID args){
    string procName=(char*)args;
    while (isAlive){
        int i=4;
        while (findRunningProcess(procName)){
            sleep(1);
            cout<<"im awake!";
        }
        system("C:\\Windows\\System32\\notepad.exe");
        cout<<"im here";
    }

    //
}


void reOpenProc(char* name) {
    cout<<"proc is opened  again!";
}

char* findRunningProcess(string process) {
/*
Function takes in a string value for the process it is looking for like ST3Monitor.exe
then loops through all of the processes that are currently running on windows.
If the process is found it is running, therefore the function returns true.
*/
    string compare;
   // bool procRunning = false;
    char* fullPath;
    HANDLE hProcessSnap;
   // PROCESSENTRY32 pe32;
    MODULEENTRY32 lpme;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        char* fullPath= NULL;
    } else {
        lpme.dwSize = sizeof(LPMODULEENTRY32);
        if (Module32First(hProcessSnap,&lpme)) { // Gets first running process
            if (lpme.szModule== process) {
                fullPath=lpme.szExePath;
            } else {
                // loop through all running processes looking for process
                while (Module32Next(hProcessSnap,&lpme)) {
                    // Set to an AnsiString instead of Char[] to make compare easier
                    compare = lpme.szModule;
                    if (compare == process)  {
                        // if found process is running, set to true and break from loop
                       // procRunning = true;
                        fullPath=lpme.szExePath;
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
                    cout<<"bye-bye";
                    break;
                }
                else{
                    cout<<"Wrong input!\n";
                    cout<<"Type 'stop' to exit:\n";
                }
            }
        }
        else{
            cout<<"process name not Exist!"<<endl;
            cout<<"exiting...";
        }

    }



}

