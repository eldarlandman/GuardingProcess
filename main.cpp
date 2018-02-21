using namespace std;
#include <iostream>
#include <thread>
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <unistd.h>
#include "funcsDeclerations.h";


    DWORD run(LPVOID procName){
    while (true){

//        while (findRunningProcess((string*)procName)){
//            sleep(5000);
//        }
    }

    //
}


void reOpenProc(char* name) {
    cout<<"proc is opened  again!";
}

bool findRunningProcess(string process) {
/*
Function takes in a string value for the process it is looking for like ST3Monitor.exe
then loops through all of the processes that are currently running on windows.
If the process is found it is running, therefore the function returns true.
*/
    string compare;
    bool procRunning = false;

    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        procRunning = false;
    } else {
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hProcessSnap, &pe32)) { // Gets first running process
            if (pe32.szExeFile == process) {
                procRunning = true;
            } else {
                // loop through all running processes looking for process
                while (Process32Next(hProcessSnap, &pe32)) {
                    // Set to an AnsiString instead of Char[] to make compare easier
                    compare = pe32.szExeFile;
                    if (compare == process) {
                        // if found process is running, set to true and break from loop
                        procRunning = true;
                        break;
                    }
                }
            }
            // clean the snapshot object
            CloseHandle(hProcessSnap);
        }
    }

    return procRunning;
}

///////////////////////////////////////////////////////////
//TODO: change all functions to start with capital letter;
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
            //main process wait for "guard" to finish
            while(cin>>input){
                if (input=="stop"){
                    cout<<"bye-bye";
                    break;
                }
                else{
                    cout<<"nothing happened";
                }
            }
        }
        else{
            cout<<"process name not Exist!"<<endl;
            cout<<"exiting...";
        }

    }



}

