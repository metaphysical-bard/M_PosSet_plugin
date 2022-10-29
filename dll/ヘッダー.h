#pragma once
#include "windows.h"

//ファイルから読み込む関数
bool read_M(char* t, int tsize, HANDLE fp, DWORD* dw) {
    if (tsize != 0) {
        if (ReadFile(fp, t, tsize, dw, NULL) == 0) {
            return false;
        }
        if (dw == 0) {
            return false;
        }
    }
    return true;
}

//ファイルに書き込む関数
bool write_M(char* t, int tsize, HANDLE fp, DWORD* dw) {
    if (tsize != 0) {
        if (WriteFile(fp, t, tsize, dw, NULL) == 0) {
            return false;
        }
        if (dw == 0) {
            return false;
        }
    }
    return true;
}