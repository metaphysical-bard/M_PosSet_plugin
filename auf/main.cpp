//----------------------------------------------------------------------------------
//		M_PosSet_plugin.auf
//----------------------------------------------------------------------------------
/*

�A�C�f�A
�Eindex�Ǘ�
�Eindex�\��
�Eindex��main�E�B���h�E�ɕ\��
�Eindex swap
�E�`�揈���y�ʉ�(�����������ʓI�Ɏg���K�v�����邽�߁A���ɉ�肻���ȋC������B
�E�u���[���n���̃A���S���Y������
�E�x�W�F�Ȑ��́A��Ō��������ق������������B�o�O��������ۂ�
�E�x�W�F��0���Z������̂Ō�ŏC�����Ȃ��Ƃ����Ȃ�
					char t[100];
					wsprintf(t, "%d %d %d", Mpos.n, pos[i].n, i);
					MessageBox(NULL, t, "test.exe", MB_OKCANCEL);
�E�f�[�^�Ǘ��p�̃t�H���_�����B����ɔ����A�f�[�^�Ǘ��p�̎d�g�݂��l����K�v������B
���Ԃ�Apath�𒷂����Ă������悤�ɕύX����K�v�����肻��

�E�t���O���Ǘ����邽�߂̃t�B���^�v���O�C����ǉ�
�E�ԍ�
�E����
�E�x�W�F�Ȑ�
�E������

�E�f�[�^�t�@�C������͐ԂƂ��́Z�𐄂��悤�ɂ���B
�E�`�F�b�N�{�b�N�X�ŁA���̕\���Ƃ���\�����ł���悤�ɂ���B
�E�g���b�N�o�[�ŏk�ڒ����ł���悤�ɂ���B���Ԃ�Aw,h���擾������Ɋ|���Z���邾���ő��v���Ǝv���B
�E����ɔ����A�`�F�b�N�{�b�N�X�ŉ�ʗ̈�\���̗L���̃{�^���Ƃ��������������������B�̈�F�́B�B�B���`��A������������

�Evector�Ńo�O������B�R���p�C���̍œK���ɂ��e���B
�E���L�������Ƀp�X��ۑ����āAdll�œǂݍ��߂�悤�ɂ���B
*/

#include <windows.h>
#include "filter.h"
//#include <lua.hpp>//plugin��lua dll�̓����͉\�Ȃ̂��ȁA�A�A
#include "draw.hpp"
windraw obj;
#include "memory.hpp"
Memory_M sm;//Mname�ɍ��W�̐����AMname2�ɍ��W�z���
Memory_M bm;

const char* Mname = "M_PosSet_AVIUTL_plugin_info";
const char* Mname2 = "M_PosSet_AVIUTL_plugin_data";
int BMnumber = -1;
bool PATH_USE = false;

BOOL func_WndProcB(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void* editp, FILTER* fp);//�Ȃ��ƃG���[�ɂȂ�


//---------------------------------------------------------------------
//		�t�B���^�\���̒�`
//---------------------------------------------------------------------
FILTER_DLL filter = {
	FILTER_FLAG_EX_INFORMATION //�t�B���^�̃t���O�c�炵��
	| FILTER_FLAG_ALWAYS_ACTIVE //�t�B���^����ɃA�N�e�B�u�ɂ��܂�
	| FILTER_FLAG_MAIN_MESSAGE //func_WndProc()��WM_FILTER_MAIN_???�̃��b�Z�[�W�𑗂�悤�ɂ��܂�
	| FILTER_FLAG_DISP_FILTER  //�\���t�B���^�ɂ��܂�
	| FILTER_FLAG_WINDOW_THICKFRAME, //�T�C�Y�ύX�\�ȃE�B���h�E�����܂�
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


//�t���O�Ǘ��p�v���O�C��
#define	TRACK_N	3														//	�g���b�N�o�[�̐�
TCHAR* track_name[] = { (char*)"�g�嗦", (char*)"�����x", (char*)"�x���x"};	//	�g���b�N�o�[�̖��O
int		track_default[] = { 100, 200, 30 };	//	�g���b�N�o�[�̏����l
int		track_s[] = { 1, 40, 1 };	//	�g���b�N�o�[�̉����l
int		track_e[] = { 1000, 255, 600 };	//	�g���b�N�o�[�̏���l

#define	CHECK_N	6												//	�`�F�b�N�{�b�N�X�̐�
TCHAR* check_name[] = { (char*)"�ԍ��\��", (char*)"����" ,(char*)"�x�W�F�Ȑ�", (char*)"������", (char*)"�g�\��", (char*)"���o�̓{�^���\��" };//	�`�F�b�N�{�b�N�X�̖��O
int		check_default[] = { 0, 0, 0, 0, 0, 0 };				//	�`�F�b�N�{�b�N�X�̏����l (�l��0��1)

FILTER_DLL filter2 = {
	FILTER_FLAG_EX_INFORMATION,	//	�t�B���^�̃t���O
	//| FILTER_FLAG_WINDOW_SIZE,			// �ݒ�E�B���h�E�̃T�C�Y���w��o����悤�ɂ��܂�
0,0,						//	�ݒ�E�C���h�E�̃T�C�Y (FILTER_FLAG_WINDOW_SIZE�������Ă��鎞�ɗL��)
(char*)"M_PosSet_ForFlag_ver.1.0",			//	�t�B���^�̖��O
TRACK_N,					//	�g���b�N�o�[�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
track_name,					//	�g���b�N�o�[�̖��O�S�ւ̃|�C���^
track_default,				//	�g���b�N�o�[�̏����l�S�ւ̃|�C���^
track_s,track_e,			//	�g���b�N�o�[�̐��l�̉������ (NULL�Ȃ�S��0�`256)
CHECK_N,					//	�`�F�b�N�{�b�N�X�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
check_name,					//	�`�F�b�N�{�b�N�X�̖��O�S�ւ̃|�C���^
check_default,				//	�`�F�b�N�{�b�N�X�̏����l�S�ւ̃|�C���^
NULL,					//	�t�B���^�����֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
NULL,						//	�J�n���ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
NULL,						//	�I�����ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
NULL,						//	�ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
func_WndProcB,						//	�ݒ�E�B���h�E�ɃE�B���h�E���b�Z�[�W���������ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
NULL,NULL,					//	�V�X�e���Ŏg���܂��̂Ŏg�p���Ȃ��ł�������
NULL,						//  �g���f�[�^�̈�ւ̃|�C���^ (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
NULL,						//  �g���f�[�^�T�C�Y (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
(char*)"M_PosSet_ForFlag_ver.1.0",
//  �t�B���^���ւ̃|�C���^ (FILTER_FLAG_EX_INFORMATION�������Ă��鎞�ɗL��)
NULL,						//	�Z�[�u���J�n����钼�O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
NULL,						//	�Z�[�u���I���������O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
};



//---------------------------------------------------------------------
//		�t�B���^�\���̂̃|�C���^��n���֐�
//---------------------------------------------------------------------
FILTER_DLL* filter_list[] = { &filter2 ,&filter,NULL };
EXTERN_C FILTER_DLL __declspec(dllexport)** __stdcall GetFilterTableList(void)
{
	return (FILTER_DLL**)&filter_list;
}


//�������ɏ�������ł����֐�
bool write_memory(FILTER* fp) {
	//sm���J��
	if (sm.p == NULL) func_init(fp);
	if (sm.p == NULL) return true;
	if (PATH_USE == false) func_init(fp);
	if (sm.p == NULL) return true;
	if (PATH_USE == false) return true;

	//�v�f����sm�ɏ�������
	int n = obj.Pwrite(true, sm.p);

	//bm���J��
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

	//���L�������ɏ�������ł���
	if (bm.p != NULL)
		obj.Pwrite(false, bm.p);
}


//�t�B���^�֐�
BOOL func_proc(FILTER* fp, FILTER_PROC_INFO* fpip)
{
	//�E�B���h�E�T�C�Y���擾����
	obj.mainwh(fpip->w, fpip->h);

	//�������ɏ�������
	write_memory(fp);

	return TRUE;
}


//---------------------------------------------------------------------
//		������
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

	CreateDirectory("M_PosSet_Data", //���̖��O�Ńf�B���N�g�������
					NULL);//�Z�L�����e�B�֘A�BNULL���Ǝ����ł���Ă����݂���

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
		MessageBox(NULL, "�G���[�ł��B", "M_PosSet_plugin", MB_OKCANCEL);
		PATH_USE = false;
	}

	return TRUE;
}


//---------------------------------------------------------------------
//		�I��
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
	//	TRUE��Ԃ��ƑS�̂��ĕ`�悳���
	//�n���h���Ƃ���fp�̂��̂��g�p���Ȃ���΂Ȃ�Ȃ��炵��

	switch (message) {

		//�`��
	case WM_PAINT:
		obj.t(fp, editp, lParam);
		break;

		//�E�B���h�E���\���A�܂��͔�\��
	case WM_FILTER_CHANGE_WINDOW:
		obj.FreeMemoryData();
		break;

		//�}�E�X���ړ�
	case WM_MOUSEMOVE:
		obj.p(lParam);
		obj.t(fp, editp, lParam);
		break;

		//���_�u���N���b�N
	case WM_LBUTTONDBLCLK:
		obj.LBclick(lParam);
		obj.t(fp, editp, lParam);
		break;

		//���N���b�N�X�^�[�g
	case WM_LBUTTONDOWN:
		obj.Lclick(fp->hwnd, lParam, true);
		obj.t(fp, editp, lParam);
		break;

		//���N���b�N�I��
	case WM_LBUTTONUP:
		obj.Lclick(NULL, lParam, false);
		obj.t(fp, editp, lParam);
		break;

		//�E�N���b�N�X�^�[�g
	case WM_RBUTTONDOWN:
		if (1) {//bool k�ŏo��G���[�΍�
			bool k = obj.Rclick(lParam);
			if (k == false) {//�Ȃɂ��Ȃ��ꏊ���E�N���b�N�ōĕ`��v��
				//�������ɏ�������
				write_memory(fp);
				obj.t(fp, editp, lParam);
				return TRUE;
			}
			else {
				obj.t(fp, editp, lParam);
			}
		}
		break;

		//���C���E�B���h�E�̃}�E�X�̓����B
	case WM_FILTER_MAIN_MOUSE_MOVE:
		obj.mainwind(lParam);
		obj.t(fp, editp, lParam);
		break;

		//�w�i�̕`��������������Ƃɂ���B
	case WM_ERASEBKGND:
		return FALSE;
	}
	return FALSE;
}




//////�t���O�Ǘ��p�̃v���O�C���̊֐�

//---------------------------------------------------------------------
//		WndProc
//---------------------------------------------------------------------
BOOL func_WndProcB(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void* editp, FILTER* fp)
{
	obj.flag_set_func(fp);
	return FALSE;
}