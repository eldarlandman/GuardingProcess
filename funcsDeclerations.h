//
// Created by Eldar Landman on 22/02/2018.
//
#include <string>

#ifndef UNTITLED1_FUNCSDECLERATIONS_H
#define UNTITLED1_FUNCSDECLERATIONS_H



void reOpenProc(char* procName);
char* findRunningProcess(string process);
bool isAlive=true;
DWORD run(LPVOID args); //LPVOID for compatibility between different versions of Windows (LPVOID aka void*)
#endif //UNTITLED1_FUNCSDECLERATIONS_H
