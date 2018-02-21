#include <iostream>
#include <iostream>
#include <thread>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
using namespace std;

bool isAlive=true;

void reOpenProc(int name);

bool GuardedProcIsAlive();

bool processExists(HANDLE procName){

    DWORD pID = GetProcessId(procName);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    return hProcess!= nullptr;
}


void run(string procName){

    string input;
    DWORD pID = GetProcessId(procName);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    while (isAlive){

        cin>>input;
        if (input=="stop"){
            isAlive=false;
            break;
        }
        while (GuardedProcIsAlive()){
            Sleep(5000);
        }
        reOpenProc(procName);


    }

}

bool GuardedProcIsAlive() {
    return false;
}

void reOpenProc(string name) {

}


///////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        cout << "invalid number of arguments";
    }
    else{
        string procName(argv[1]);
        HANDLE handle = new string("Notepad");
        if (processExists(handle)){
            std::thread guard(run, procName);
            //main process wait for "guard" to finish
            guard.join();
        }
        else{
            cout<<"process name not Exist!"endl;
            cout<<"exiting...";
        }

    }



}

