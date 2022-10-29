//----------------------------------------------------------------------------------
//		M_PosSet_plugin.auf
//----------------------------------------------------------------------------------
/*

アイデア
・index管理
・index表示
・indexをmainウィンドウに表示
・index swap
・描画処理軽量化(メモリを効果的に使う必要があるため、後手に回りそうな気がする。
・ブレゼンハムのアルゴリズム実装
・ベジェ曲線は、後で見直したほうがいいかも。バグがあるっぽい
・ベジェは0除算があるので後で修正しないといけない
					char t[100];
					wsprintf(t, "%d %d %d", Mpos.n, pos[i].n, i);
					MessageBox(NULL, t, "test.exe", MB_OKCANCEL);
・データ管理用のフォルダを作る。それに伴い、データ管理用の仕組みを考える必要がある。
たぶん、pathを長くしてもいいように変更する必要がありそう

・フラグを管理するためのフィルタプラグインを追加
・番号
・線分
・ベジェ曲線
・半透明

・データファイル操作は赤とか青の〇を推すようにする。
・チェックボックスで、↑の表示とか非表示をできるようにする。
・トラックバーで縮尺調整できるようにする。たぶん、w,hを取得した後に掛け算するだけで大丈夫だと思われ。
・それに伴い、チェックボックスで画面領域表示の有無のボタンとかあった方がいいかも。領域色は。。。う～ん、何がいいんやろ

・vectorでバグがある。コンパイラの最適化による影響。
・共有メモリにパスを保存して、dllで読み込めるようにする。
*/

#include <windows.h>
#include "filter.h"
//#include <lua.hpp>//pluginとlua dllの同棲は可能なのかな、、、
#include "draw.hpp"
windraw obj;
#include "memory.hpp"
Memory_M sm;//Mnameに座標の数を、Mname2に座標配列を
Memory_M bm;

const char* Mname = "M_PosSet_AVIUTL_plugin_info";
const char* Mname2 = "M_PosSet_AVIUTL_plugin_data";
int BMnumber = -1;
bool PATH_USE = false;

BOOL func_WndProcB(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void* editp, FILTER* fp);//ないとエラーになる


//---------------------------------------------------------------------
//		フィルタ構造体定義
//---------------------------------------------------------------------
FILTER_DLL filter = {
	FILTER_FLAG_EX_INFORMATION //フィルタのフラグ…らしい
	| FILTER_FLAG_ALWAYS_ACTIVE //フィルタを常にアクティブにします
	| FILTER_FLAG_MAIN_MESSAGE //func_WndProc()にWM_FILTER_MAIN_???のメッセージを送るようにします
	| FILTER_FLAG_DISP_FILTER  //表示フィルタにします
	| FILTER_FLAG_WINDOW_THICKFRAME, //サイズ変更可能なウィンドウを作ります
	NULL,NULL,
	(char*)"M_PosSet_plugin",
	NULL,NULL,NULL,NULL,NULL,
	NULL,NULL,NULL,
	func_proc,
	func_init,
	func_exit,
	NULL,
	func_WndProc,
	NULL,NULL,
	NULL,
	NULL,
	(char*)"M_PosSet_plugin_ver.1.0",
	NULL,NULL,
	NULL,NULL,NULL,NULL,
	NULL,
};


//フラグ管理用プラグイン
#define	TRACK_N	3														//	トラックバーの数
TCHAR* track_name[] = { (char*)"拡大率", (char*)"透明度", (char*)"ベ精度"};	//	トラックバーの名前
int		track_default[] = { 100, 200, 30 };	//	トラックバーの初期値
int		track_s[] = { 1, 40, 1 };	//	トラックバーの下限値
int		track_e[] = { 1000, 255, 600 };	//	トラックバーの上限値

#define	CHECK_N	6												//	チェックボックスの数
TCHAR* check_name[] = { (char*)"番号表示", (char*)"線分" ,(char*)"ベジェ曲線", (char*)"半透明", (char*)"枠表示", (char*)"入出力ボタン表示" };//	チェックボックスの名前
int		check_default[] = { 0, 0, 0, 0, 0, 0 };				//	チェックボックスの初期値 (値は0か1)

FILTER_DLL filter2 = {
	FILTER_FLAG_EX_INFORMATION,	//	フィルタのフラグ
	//| FILTER_FLAG_WINDOW_SIZE,			// 設定ウィンドウのサイズを指定出来るようにします
0,0,						//	設定ウインドウのサイズ (FILTER_FLAG_WINDOW_SIZEが立っている時に有効)
(char*)"M_PosSet_ForFlag_ver.1.0",			//	フィルタの名前
TRACK_N,					//	トラックバーの数 (0なら名前初期値等もNULLでよい)
track_name,					//	トラックバーの名前郡へのポインタ
track_default,				//	トラックバーの初期値郡へのポインタ
track_s,track_e,			//	トラックバーの数値の下限上限 (NULLなら全て0～256)
CHECK_N,					//	チェックボックスの数 (0なら名前初期値等もNULLでよい)
check_name,					//	チェックボックスの名前郡へのポインタ
check_default,				//	チェックボックスの初期値郡へのポインタ
NULL,					//	フィルタ処理関数へのポインタ (NULLなら呼ばれません)
NULL,						//	開始時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
NULL,						//	終了時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
NULL,						//	設定が変更されたときに呼ばれる関数へのポインタ (NULLなら呼ばれません)
func_WndProcB,						//	設定ウィンドウにウィンドウメッセージが来た時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
NULL,NULL,					//	システムで使いますので使用しないでください
NULL,						//  拡張データ領域へのポインタ (FILTER_FLAG_EX_DATAが立っている時に有効)
NULL,						//  拡張データサイズ (FILTER_FLAG_EX_DATAが立っている時に有効)
(char*)"M_PosSet_ForFlag_ver.1.0",
//  フィルタ情報へのポインタ (FILTER_FLAG_EX_INFORMATIONが立っている時に有効)
NULL,						//	セーブが開始される直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
NULL,						//	セーブが終了した直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
};



//---------------------------------------------------------------------
//		フィルタ構造体のポインタを渡す関数
//---------------------------------------------------------------------
FILTER_DLL* filter_list[] = { &filter2 ,&filter,NULL };
EXTERN_C FILTER_DLL __declspec(dllexport)** __stdcall GetFilterTableList(void)
{
	return (FILTER_DLL**)&filter_list;
}


//メモリに書き込んでいく関数
bool write_memory(FILTER* fp) {
	//smを開く
	if (sm.p == NULL) func_init(fp);
	if (sm.p == NULL) return true;
	if (PATH_USE == false) func_init(fp);
	if (sm.p == NULL) return true;
	if (PATH_USE == false) return true;

	//要素数をsmに書き込み
	int n = obj.Pwrite(true, sm.p);

	//bmを開く
	if (n <= 0) return true;
	if (n != BMnumber || bm.p == NULL) {
		bm.Memorydelete();
		bm.Mname = (char*)Mname2;
		bm.MemorySize = n * 2 * sizeof(double);
		if (bm.MemoryCreate() == true) {
			BMnumber = n;
		}
		else {
			return true;
		}
	}

	//共有メモリに書き込んでいく
	if (bm.p != NULL)
		obj.Pwrite(false, bm.p);
}


//フィルタ関数
BOOL func_proc(FILTER* fp, FILTER_PROC_INFO* fpip)
{
	//ウィンドウサイズを取得する
	obj.mainwh(fpip->w, fpip->h);

	//メモリに書き込む
	write_memory(fp);

	return TRUE;
}


//---------------------------------------------------------------------
//		初期化
//---------------------------------------------------------------------
BOOL func_init(FILTER* fp)
{
	sm.Memorydelete();
	bm.Memorydelete();

	sm.Mname = (char*)Mname;
	sm.MemorySize = 4 + MAX_PATH;

	int* a;
	if (sm.MemoryCreate() == true) {
		a = reinterpret_cast<int*>(sm.p);
		*a = 0;
	}
	else {
		PATH_USE = false;
		return TRUE;
	}

	CreateDirectory("M_PosSet_Data", //この名前でディレクトリを作る
					NULL);//セキュリティ関連。NULLだと自動でやってくれるみたい

	char t[MAX_PATH];
	if (GetFullPathNameA("M_PosSet_Data", MAX_PATH, t, NULL) != FALSE) {
		//MessageBox(NULL, t, "test.exe", MB_OKCANCEL);
		a++;
		char* b = reinterpret_cast<char*>(a);
		for (int i = 0; i < MAX_PATH; i++) {
			*(b + i) = t[i];
		}
		PATH_USE = true;
	}
	else {
		MessageBox(NULL, "エラーです。", "M_PosSet_plugin", MB_OKCANCEL);
		PATH_USE = false;
	}

	return TRUE;
}


//---------------------------------------------------------------------
//		終了
//---------------------------------------------------------------------
BOOL func_exit(FILTER* fp)
{
	sm.Memorydelete();
	bm.Memorydelete();
	return TRUE;
}


//---------------------------------------------------------------------
//		WndProc
//---------------------------------------------------------------------
BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void* editp, FILTER* fp)
{
	//	TRUEを返すと全体が再描画される
	//ハンドルとかはfpのものを使用しなければならないらしい

	switch (message) {

		//描画
	case WM_PAINT:
		obj.t(fp, editp, lParam);
		break;

		//ウィンドウが表示、または非表示
	case WM_FILTER_CHANGE_WINDOW:
		obj.FreeMemoryData();
		break;

		//マウスが移動
	case WM_MOUSEMOVE:
		obj.p(lParam);
		obj.t(fp, editp, lParam);
		break;

		//左ダブルクリック
	case WM_LBUTTONDBLCLK:
		obj.LBclick(lParam);
		obj.t(fp, editp, lParam);
		break;

		//左クリックスタート
	case WM_LBUTTONDOWN:
		obj.Lclick(fp->hwnd, lParam, true);
		obj.t(fp, editp, lParam);
		break;

		//左クリック終了
	case WM_LBUTTONUP:
		obj.Lclick(NULL, lParam, false);
		obj.t(fp, editp, lParam);
		break;

		//右クリックスタート
	case WM_RBUTTONDOWN:
		if (1) {//bool kで出るエラー対策
			bool k = obj.Rclick(lParam);
			if (k == false) {//なにもない場所を右クリックで再描画要請
				//メモリに書き込む
				write_memory(fp);
				obj.t(fp, editp, lParam);
				return TRUE;
			}
			else {
				obj.t(fp, editp, lParam);
			}
		}
		break;

		//メインウィンドウのマウスの動き。
	case WM_FILTER_MAIN_MOUSE_MOVE:
		obj.mainwind(lParam);
		obj.t(fp, editp, lParam);
		break;

		//背景の描画を処理したことにする。
	case WM_ERASEBKGND:
		return FALSE;
	}
	return FALSE;
}




//////フラグ管理用のプラグインの関数

//---------------------------------------------------------------------
//		WndProc
//---------------------------------------------------------------------
BOOL func_WndProcB(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void* editp, FILTER* fp)
{
	obj.flag_set_func(fp);
	return FALSE;
}