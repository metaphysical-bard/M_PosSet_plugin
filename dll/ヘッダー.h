#pragma once
#include "windows.h"

//�t�@�C������ǂݍ��ފ֐�
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

//�t�@�C���ɏ������ފ֐�
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