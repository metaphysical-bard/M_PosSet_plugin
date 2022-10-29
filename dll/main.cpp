
#include "windows.h"
#include <lua.hpp>
#include "memory.hpp"
#include "�w�b�_�[.h"

const char* Mname = "M_PosSet_AVIUTL_plugin_info";
const char* Mname2 = "M_PosSet_AVIUTL_plugin_data";

int p2(lua_State* L) {

    Memory_M sm;
    Memory_M bm;

    sm.Mname = (char*)Mname;
    sm.MemorySize = 4 + MAX_PATH;

    if (sm.MemoryCreate() == true) {
        int* n = reinterpret_cast<int*>(sm.p);

        bm.Mname = (char*)Mname2;
        bm.MemorySize = *n * 2 * 8;
        if (bm.MemoryCreate() == true) {
            double* pos = reinterpret_cast<double*>(bm.p);
            for (int i = 0; i < *n * 2; i++) {
                lua_pushnumber(L, pos[i]);
                lua_rawseti(L, 1, i + 1);
            }
        }
    }

    sm.Memorydelete();
    bm.Memorydelete();

    return 0;
}

int p3(lua_State* L) {

    Memory_M sm;
    Memory_M bm;

    sm.Mname = (char*)Mname;
    sm.MemorySize = 4 + MAX_PATH;

    if (sm.MemoryCreate() == true) {
        int* n = reinterpret_cast<int*>(sm.p);

        bm.Mname = (char*)Mname2;
        bm.MemorySize = *n * 2 * 8;
        if (bm.MemoryCreate() == true) {
            double* pos = reinterpret_cast<double*>(bm.p);
            for (int i = 0; i < *n; i++) {

                lua_pushnumber(L, pos[i * 2]);
                lua_rawseti(L, 1, i * 3 + 1);

                lua_pushnumber(L, pos[i * 2 + 1]);
                lua_rawseti(L, 1, i * 3 + 2);

                lua_pushnumber(L, 0);
                lua_rawseti(L, 1, i * 3 + 3);
            }
        }
    }

    sm.Memorydelete();
    bm.Memorydelete();

    return 0;
}

int write(lua_State* L) {
    //(FileName,n,t)
    Memory_M sm;
    sm.Mname = (char*)Mname;
    sm.MemorySize = 4 + MAX_PATH;
    char FilePath[MAX_PATH];
    ZeroMemory(FilePath, MAX_PATH);

    if (sm.MemoryCreate() == true) {//path���쐬����
        int* a = reinterpret_cast<int*>(sm.p);
        char* t = reinterpret_cast<char*>(a + 1);

        size_t s = (size_t)MAX_PATH;
        const char* FileName = lua_tolstring(L, 1, &s);

        int i;
        for (i = 0; i < MAX_PATH; i++) {
            if (t[i] == '\0') break;
            else FilePath[i] = t[i];
        }
        for (int j = 0; j < MAX_PATH - i; j++) {
            if (*(FileName + j) == '\0') {
                FilePath[i + j] = '\0';
                break;
            }
            else
                FilePath[i + j] = FileName[j];
        }
    }
    else {
        return 0;
    }
    sm.Memorydelete();

    int n = lua_tointeger(L, 2);

    if (FilePath[0]) {//��������
        HANDLE fp;
        if ((fp = CreateFileA(FilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
            DWORD dw;
            char t[100];
            ZeroMemory(&t, 100);
            int tsize = wsprintfA(t, "n=%10d\r\n", n);
            if (write_M(t, tsize, fp, &dw) == false) {
                CloseHandle(fp);//file�����
                return true;
            }

            if (n == 0) {
                CloseHandle(fp);//file�����
                return true;
            }

            int a, b, c, d, i;
            int x, y;
            for (i = 0; i < n - 1; i++) {
                lua_rawgeti(L, 3, i * 2 + 1);
                x = lua_tonumber(L, -1);
                lua_pop(L, 1);

                lua_rawgeti(L, 3, i * 2 + 2);
                y = lua_tonumber(L, -1);
                lua_pop(L, 1);

                a = (int)x;
                b = ((x - a) * 1000000);
                if (b < 0) b *= -1;
                c = (int)y;
                d = ((y - c) * 1000000);
                if (d < 0) d *= -1;
                tsize = wsprintfA(t, "%14d.%06d,%14d.%06d,\r\n", a, b, c, d);
                if (write_M(t, tsize, fp, &dw) == false) {
                    CloseHandle(fp);//file�����
                    return true;
                }
            }

            i = n - 1;

            lua_rawgeti(L, 3, i * 2 + 1);
            x = lua_tonumber(L, -1);
            lua_pop(L, 1);

            lua_rawgeti(L, 3, i * 2 + 2);
            y = lua_tonumber(L, -1);
            lua_pop(L, 1);

            a = (int)x;
            b = ((x - a) * 1000000);
            if (b < 0) b *= -1;
            c = (int)y;
            d = ((y - c) * 1000000);
            if (d < 0) d *= -1;
            tsize = wsprintfA(t, "%14d.%06d,%14d.%06d\r\n", a, b, c, d);
            if (write_M(t, tsize, fp, &dw) == false) {
                CloseHandle(fp);//file�����
                return true;
            }

            CloseHandle(fp);//file�����
        }
    }

    return 0;
}

int read(lua_State* L) {
    //(FileName,t)
    Memory_M sm;
    sm.Mname = (char*)Mname;
    sm.MemorySize = 4 + MAX_PATH;
    char FilePath[MAX_PATH];
    ZeroMemory(FilePath, MAX_PATH);

    if (sm.MemoryCreate() == true) {//path���쐬����
        int* a = reinterpret_cast<int*>(sm.p);
        char* t = reinterpret_cast<char*>(a + 1);

        size_t s = (size_t)MAX_PATH;
        const char* FileName = lua_tolstring(L, 1, &s);

        int i;
        for (i = 0; i < MAX_PATH; i++) {
            if (t[i] == '\0') break;
            else FilePath[i] = t[i];
        }
        for (int j = 0; j < MAX_PATH - i; j++) {
            if (*(FileName + j) == '\0') {
                FilePath[i + j] = '\0';
                break;
            }
            else
                FilePath[i + j] = FileName[j];
        }
    }
    else {
        return 0;
    }
    sm.Memorydelete();

    if (FilePath[0]) {//��݂���
        HANDLE fp;
        if ((fp = CreateFileA(FilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {

            DWORD dw;
            char t[30];
            if (read_M(t, 2, fp, &dw) == false) {//n=
                CloseHandle(fp);//file�����
                return true;
            }

            int n;
            if (read_M(t, 14, fp, &dw) == false) {//pos�̐�(10���{���s�R�[�h4
                CloseHandle(fp);//file�����
                return true;
            }
            else {
                n = atoi(t);
            }

            int i;
            if (n > 0) {
                for (i = 0; i < n - 1; i++) {
                    if (read_M(t, 22, fp, &dw) == false) {//���l20��(�J���}�����21�o�C�g)�{�u,�v
                        CloseHandle(fp);//file�����
                        return true;
                    }
                    else {
                        lua_pushnumber(L, atof(t));
                        lua_rawseti(L, 2, i * 2 + 1);
                    }
                    if (read_M(t, 24, fp, &dw) == false) {//���l20��(�J���}�����21�o�C�g)�{�u,�v�{���s�R�[�h
                        CloseHandle(fp);//file�����
                        return true;
                    }
                    else {
                        lua_pushnumber(L, atof(t));
                        lua_rawseti(L, 2, i * 2 + 2);
                    }

                }

                i = n - 1;
                if (read_M(t, 22, fp, &dw) == false) {//���l20��(�J���}�����21�o�C�g)�{�u,�v
                    CloseHandle(fp);//file�����
                    return true;
                }
                else {
                    lua_pushnumber(L, atof(t));
                    lua_rawseti(L, 2, i * 2 + 1);
                }
                if (read_M(t, 21, fp, &dw) == false) {//���l20��(�J���}�����21�o�C�g)
                    CloseHandle(fp);//file�����
                    return true;
                }
                else {
                    lua_pushnumber(L, atof(t));
                    lua_rawseti(L, 2, i * 2 + 2);
                }
            }

            CloseHandle(fp);//file�����
        }
    }

    return 0;
}

static luaL_Reg functions[] = {
    {"p2",p2},//(x,y)���W��lua��
    {"p3",p3},//(x,y,0)���W��lua��
    {"write",write},//�z�񂩂�t�@�C�����쐬����
    {"read",read},//�t�@�C������z����쐬����
    {nullptr,nullptr}
};

extern "C" {
    __declspec(dllexport) int luaopen_M_PosSet_Module(lua_State* L) {
        luaL_register(L, "M_PosSet_Module", functions);
        return 1;
    }
}