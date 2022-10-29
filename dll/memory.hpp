/*
共有メモリ関連
*/
#pragma once
#include <windows.h>


class Memory_M
{

public:

    //共有メモリサイズ
    char* Mname = (char*)"name";//名前
    DWORD MemorySize = 100;//サイズ
    HANDLE Mhandle;//ハンドル
    void* p = NULL;//割り当てメモリのポインタ


    //共有メモリ作成とマッピング
    bool MemoryCreate() {

        //共有メモリ作成
        Mhandle = CreateFileMappingA(
            INVALID_HANDLE_VALUE//共有メモリの場合は、0xffffffff(INVALID_HANDLE_VALUE)で固定
            , NULL// SECURITY_ATTRIBUTES構造体(セキュリティ関連らしい、
            , PAGE_READWRITE//保護属性(これは読み書き両方可
            , 0//メモリの最小サイズ
            , MemorySize//最大サイズ
            , Mname//共有メモリの名前
        );
        if (NULL == Mhandle) {
            //MessageBox(NULL, "共有メモリ作成失敗～", "a", MB_OK);
            return false;
        }

        //共有メモリマッピング
        p = MapViewOfFile(
            Mhandle//ハンドル
            , FILE_MAP_WRITE//保護属性(これは読み書き両方可
            , 0//オフセット
            , 0//オフセット
            , MemorySize//サイズ
        );
        if (p == NULL) {
            //MessageBox(NULL, "共有メモリマッピング失敗～", "a", MB_OK);
            return false;
        }

        return true;
    }

    //共有メモリ削除
    void Memorydelete() {
        if (p != NULL) {// 共有マッピングの解除
            UnmapViewOfFile(p);
        }
        if (Mhandle != NULL) {// ファイルマッピングオブジェクトハンドルの解放
            CloseHandle(Mhandle);
        }
    }

};