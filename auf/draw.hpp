/*
�E�B���h�E�`��֘A
*/

#pragma once
#include <windows.h>
#include <math.h>
#include <tchar.h>
#include <new>
#include <vector>

class windraw
{
    //�E�B���h�E�̃T�C�Y
    int w = 1;
    int h = 1;
    int dw = 0;
    int dh = 0;

    //main�E�B���h�E�̃T�C�Y
    int mainw = 100;
    int mainh = 100;
    int maindw = 0;
    int maindh = 0;

    struct Pixel_RGB {
        unsigned char b;
        unsigned char g;
        unsigned char r;
    };

    //�摜�f�[�^
    Pixel_RGB* data = nullptr;

    BITMAPINFO d;//bmp file�̍\����

    //�\���̂�ݒ�(�w�b�_�[
    void set() {
        ZeroMemory(&d, sizeof(d));
        d.bmiHeader.biSize = sizeof(d.bmiHeader);
        d.bmiHeader.biWidth = w;
        d.bmiHeader.biHeight = h;
        d.bmiHeader.biPlanes = 1;
        d.bmiHeader.biBitCount = 24;
        d.bmiHeader.biCompression = BI_RGB;
        d.bmiHeader.biPlanes = 1;
    }

    //���W�i�[�̍\����
    struct POS_XY {
        double x;//x���W(aul��
        double y;//y���W(aul��
        int px;//�\���p
        int py;//�\���p
        int n;//index�Ǘ��p
    };

    //�}�E�X���W
    POS_XY Mpos = { 0,0,0,0,0 };
    bool Mpos_umu = false;//�}�E�X��pos�̍��W�������N���邩�ǂ���
    bool Mpos_main = false;//�}�E�X�����C���E�B���h�E�ɂ��邩�ǂ���
    bool Mpos_hani = false;//�͈͑I���̗L��
    bool Mpos_shani = false;//�͈͑I���ŏI���ʒu���g�����̗L��
    bool Mpos_ihani = false;//�͈͈�Ĉړ�
    int Mpos_dx = -1;//�͈͑I���̍ۂ̏����ʒu
    int Mpos_dy = -1;
    int Mpos_sx = -2;//�͈͑I���̍ۂ̏I���ʒu
    int Mpos_sy = -2;

    //�I��͈͂̎n�_�ƏI�_
    int sx = 0, sy = 0, fx = 0, fy = 0;

    //�͈͈�Ĉړ��̂Ƃ��̈ړ���
    int bx = 0, by = 0;

    //���W��ۑ�����vector
    std::vector<POS_XY> pos;

    //�`�掞�ɍ��W�𐮗����邽�߂̔z��
    float* pos_data_for_draw = nullptr;
    int pos_data_for_draw_num = 0;//��

    //index�Ǘ��p
    int POS_COUNT = 0;

    //�����e�L�X�g�f�[�^
    bool number_data[10][5][4];


    //���낢��ȃt���O
    float track_zoom = 1.0;//�g�嗦
    int alpha_atai = 255;//�����x
    int alpha_atai_d = 255;//���݂̃E�B���h�E�̓����x
    int bezie_seido = 30;//�x�W�G�Ȑ��̐��x
    bool flag_draw_number = false;//�ԍ��\���̗L��
    bool flag_draw_line = false;//���C���̗L��
    bool flag_draw_bezie = false;//�x�W�F�Ȑ��̕`��̗L��
    bool flag_draw_alpha = false;//�������̗L��
    bool flag_draw_waku = false;//�g�\���̗L��
    bool flag_draw_button = false;//���o�̓{�^���̕\��

public:

    //�t���O�Ǘ��p
    void flag_set_func(FILTER* fp) {

        track_zoom = fp->track[0] * 0.01;
        alpha_atai = fp->track[1];
        bezie_seido = fp->track[2];

        if (fp->check[0] == 1)
            flag_draw_number = true;
        else
            flag_draw_number = false;

        if (fp->check[1] == 1)
            flag_draw_line = true;
        else
            flag_draw_line = false;

        if (fp->check[2] == 1)
            flag_draw_bezie = true;
        else
            flag_draw_bezie = false;

        if (fp->check[3] == 1)
            flag_draw_alpha = true;
        else
            flag_draw_alpha = false;

        if (fp->check[4] == 1)
            flag_draw_waku = true;
        else
            flag_draw_waku = false;

        if (fp->check[5] == 1)
            flag_draw_button = true;
        else
            flag_draw_button = false;
    }

    //���{���
    float zyo2(float x, float y) {
        return x * x + y * y;
    }

    //w,h�̒l���擾
    bool wh(FILTER* fp, LPARAM lParam) {
        RECT rc;
        GetClientRect(fp->hwnd, &rc);
        w = rc.right;
        h = rc.bottom;

        //�摜�̉��̃��C���̃f�[�^�́C4 �̔{���ɑ����Ȃ���΂����Ȃ��c�Ƃ̂���
        if (w % 4 != 0)
            w = w + (4 - w % 4);

        //������
        if (w != dw || h != dh) {
            delete[] data;
            data = new(std::nothrow)Pixel_RGB[w * h];
            if (data == NULL) {
                MessageBox(NULL, "�������m�ێ��s", "M_PosSet_AVIUTL_plugin", MB_OK);
                data = nullptr;
                return false;
            }

            //�w�b�_�[��������
            set();

            dw = w;
            dh = h;

            //�\���p�̍��W���ϊ�
            for (int i = 0; i < pos.size(); i++) {
                pos[i].px = static_cast<int>(ad(pos[i].x, true));
                pos[i].py = static_cast<int>(ad(pos[i].y, false));
            }
        }

        return true;
    }


    //���C���E�B���h�E�̃T�C�Y���L�^
    void mainwh(int w, int h) {
        mainw = static_cast<int>(w * track_zoom);
        mainh = static_cast<int>(h * track_zoom);
        if (mainw != maindw || mainh != maindh) {//���W��ϊ�
            for (int i = 0; i < pos.size(); i++) {
                pos[i].x = pos[i].x * mainw / maindw;
                pos[i].y = pos[i].y * mainh / maindh;
            }
        }
        maindw = mainw;
        maindh = mainh;
    }


    //aviutl��̍��W���\���p�̍��W
    float ad(double s, bool type) {
        if (type)
            return (s + mainw / 2.0) / mainw * w;
        else
            return (s + mainh / 2.0) / mainh * h;
    }


    //�\���p�̍��W��aviutl��̍��W
    double da(int s, bool type) {
        if (type)
            return (double)s / w * mainw - mainw / 2.0;
        else
            return (double)s / h * mainh - mainh / 2.0;
    }


    //�n�_�ƏI�_�����߂�
    void sf(int cx, int cy, int dx, int dy, int* sx, int* sy, int* fx, int* fy) {
        if (dx < cx) {
            *sx = dx;
            *fx = cx;
        }
        else {
            *sx = cx;
            *fx = dx;
        }
        if (dy < cy) {
            *sy = dy;
            *fy = cy;
        }
        else {
            *sy = cy;
            *fy = dy;
        }
    }


    //���W��ݒ�
    void p(LPARAM lParam) {
        if (Mpos_main) Mpos_main = false;
        Mpos.px = LOWORD(lParam);
        Mpos.py = HIWORD(lParam);
        Mpos.x = da((double)Mpos.px, true);
        Mpos.y = da((double)Mpos.py, false);

        if (Mpos_umu == true) {
            int i = Mpos.n;
            pos[i].x = Mpos.x;
            pos[i].y = Mpos.y;
            pos[i].px = Mpos.px;
            pos[i].py = Mpos.py;
        }

    }


    //���_�u���N���b�N
    bool LBclick(LPARAM lParam) {
        p(lParam);//�}�E�X���W�̍X�V

        if (Mpos_hani == true) {//�͈͑I�𖳌�
            Mpos_hani = false;
        }

        for (int i = 0; i < pos.size(); i++) {//�����̍��W�Ɠ��ꂩ�ǂ���
            if (zyo2(pos[i].px - Mpos.px, pos[i].py - Mpos.py) < 9) {
                //�����Ȃ�A�I��
                return false;
            }
        }

        pos.push_back(Mpos);//�V�������W�ǉ�
        pos[pos.size() - 1].n = POS_COUNT;
        POS_COUNT = POS_COUNT + 1;
        return true;
    }


    //���N���b�N�̍ۂ̋���
    bool Lclick(HWND hwnd, LPARAM lParam, bool sore) {
        p(lParam);//�}�E�X���W�̍X�V
        if (sore == true) {//�N���b�N�X�^�[�g
            if (flag_draw_button == true) {//���o�̓{�^���\����
                if (Mpos.px >= 10 && Mpos.px < 16) {
                    if (Mpos.py >= 10 && Mpos.py < 16) {//���̓{�^��
                        RBclick(hwnd, true);
                        return true;
                    }
                    else if (Mpos.py >= 20 && Mpos.py < 26) {//�o�̓{�^��
                        RBclick(hwnd, false);
                        return true;
                    }
                }
            }

            if (Mpos_hani == true) {//�͈͑I��L���ȏꍇ
                //�}�E�X���͈͓��̏ꍇ
                if (Mpos.px >= sx && Mpos.px <= fx) {
                    if (Mpos.py >= sy && Mpos.py <= fy) {
                        Mpos_ihani = true;//�͈͈�Ĉړ�
                        Mpos_dx = Mpos.px;//���݂̈ʒu���L�^
                        Mpos_dy = Mpos.py;
                        return true;
                    }
                }
                Mpos_hani = false;//�I��͈͂𖳌��ɂ���
            }

            for (int i = 0; i < pos.size(); i++) {//�����̍��W�Ɠ��ꂩ�ǂ���
                if (zyo2(pos[i].px - Mpos.px, pos[i].py - Mpos.py) < 9) {
                    //�����Ȃ�A
                    Mpos.n = i;//�ԍ��̋L�^
                    pos[i].x = Mpos.x;
                    pos[i].y = Mpos.y;
                    pos[i].px = Mpos.px;
                    pos[i].py = Mpos.py;
                    Mpos_umu = true;//�L���ɂ���
                    break;
                }
            }

            if (Mpos_umu == false) {//�����̍��W�ƈ�v���Ȃ��ꍇ
                Mpos_hani = true;//�͈͑I��L����
                Mpos_shani = false;//�I�����W����
                Mpos_dx = Mpos.px;//�������W���L�^
                Mpos_dy = Mpos.py;
            }
        }
        else {//�N���b�N�I��
            if (Mpos_ihani == true) {//�͈͈�Ĉړ�
                Mpos_ihani = false;
                Mpos_hani = false;//�I��͈͂𖳌��ɂ���

                //�͈͓��̍��W���ړ�
                bx = Mpos.px - Mpos_dx;
                by = Mpos.py - Mpos_dy;
                int i = 0, j = 0;
                while (j < pos.size()) {
                    for (i = j; i < pos.size(); i++) {
                        if (pos[i].px >= sx && pos[i].px <= fx) {
                            if (pos[i].py >= sy && pos[i].py <= fy) {
                                pos[i].px = pos[i].px + bx;
                                pos[i].py = pos[i].py + by;
                                if (pos[i].px<0 || pos[i].px>w - 1 || pos[i].py<0 || pos[i].py>h - 1) {
                                    //�E�B���h�E�̊O�ɏo������W���폜
                                    POS_XY a = pos[pos.size() - 1];
                                    pos.pop_back();//�����̗v�f�폜
                                    pos[i] = a;
                                    break;
                                }
                                pos[i].x = da(pos[i].px, true);
                                pos[i].y = da(pos[i].py, false);
                            }
                        }
                    }
                    j = i;
                }
            }
            else if (Mpos_umu == true) {//���W�̒P��ړ�
                Mpos_umu = false;//�����ɂ���
            }
            else if (Mpos_hani == true) {//�͈͎w��
                Mpos_sx = Mpos.px;//�I�����W���L�^
                Mpos_sy = Mpos.py;
                Mpos_shani = true;//�I�����W���g��
            }
        }
        return true;
    }


    //�E�N���b�N�̂Ƃ�
    bool Rclick(LPARAM lParam) {
        p(lParam);//�}�E�X���W�̍X�V

        if (Mpos_hani == true) {//�͈͑I��L���ȏꍇ
            Mpos_hani = false;//�I��͈͂𖳌��ɂ���

            //�}�E�X���͈͓��̏ꍇ
            if (Mpos.px >= sx && Mpos.px <= fx) {
                if (Mpos.py >= sy && Mpos.py <= fy) {
                    //�͈͓��̍��W���폜
                    int i = 0, j = 0;
                    while (j < pos.size()) {
                        for (i = j; i < pos.size(); i++) {
                            if (pos[i].px >= sx && pos[i].px <= fx) {
                                if (pos[i].py >= sy && pos[i].py <= fy) {
                                    //���W���폜
                                    POS_XY a = pos[pos.size() - 1];
                                    pos.pop_back();//�����̗v�f�폜
                                    pos[i] = a;
                                    break;
                                }
                            }
                        }
                        j = i;
                    }
                    return true;
                }
            }
        }

        for (int i = 0; i < pos.size(); i++) {//�����̍��W�Ɠ��ꂩ�ǂ���
            if (zyo2(pos[i].px - Mpos.px, pos[i].py - Mpos.py) < 9) {
                //�����Ȃ�A���̍��W���폜
                POS_XY a = pos[pos.size() - 1];
                pos.pop_back();//�����̗v�f�폜
                pos[i] = a;
                return true;
            }
        }

        return false;
    }

    //�t�@�C������ǂݍ��ފ֐�
    bool read_M(char* t, int tsize, HANDLE fp, DWORD* dw) {
        if (tsize != 0) {
            if (ReadFile(fp, t, tsize, dw, NULL) == 0) {
                MessageBox(NULL, "�ǂݍ��ݎ��s", "M_PosSet_AVIUTL_plugin", MB_OKCANCEL);//���s��
                return false;
            }
            if (dw == 0) {
                MessageBox(NULL, "�ǂݍ��ݎ��s", "M_PosSet_AVIUTL_plugin", MB_OKCANCEL);//���s��
                return false;
            }
        }
        return true;
    }

    //�t�@�C���ɏ������ފ֐�
    bool write_M(char* t, int tsize, HANDLE fp, DWORD* dw) {
        if (tsize != 0) {
            if (WriteFile(fp, t, tsize, dw, NULL) == 0) {
                MessageBox(NULL, "�������ݎ��s", "M_PosSet_AVIUTL_plugin", MB_OKCANCEL);//file�������ݎ��s��
                return false;
            }
            if (dw == 0) {
                MessageBox(NULL, "�������ݎ��s", "M_PosSet_AVIUTL_plugin", MB_OKCANCEL);//file�������ݎ��s��
                return false;
            }
        }
        return true;
    }

    //�f�[�^�t�@�C������
    bool RBclick(HWND hwnd, bool type) {
        char FileName[MAX_PATH];
        ZeroMemory(FileName, MAX_PATH);
        OPENFILENAME t;
        ZeroMemory(&t, sizeof(OPENFILENAME));
        t.lStructSize = sizeof(OPENFILENAME);//�\���̂̃T�C�Y
        t.hwndOwner = hwnd;//�I�[�i�[�E�B���h�E�̃n���h���BNULL�ł��悫
        t.lpstrFilter = _T("TXT Files\0*.txt;\0All Files(*.*)\0*.*\0\0");//�t�@�C���̐����������邠��A
        t.nFilterIndex = 2;//lpstrFilter�̒��ł̃f�t�H��ԍ��Ŏw��B0�ł��悫
        t.lpstrFile = FileName;//�I�����ꂽ�e�L�X�g�̓���o�b�t�@
        t.nMaxFile = MAX_PATH;//�o�b�t�@�T�C�Y
        t.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;//���낢��ȃt���O
        t.lpstrDefExt = "txt";//�g���q�������ŕt���Ă����݂���
        t.nMaxFileTitle = NULL;//�ȂɂɎg���̂��킩���
        t.lpstrFileTitle = NULL;//�㓯
        t.lpstrTitle = NULL;//�^�C�g���o�[�̕�����BNULL�ł��悫
        t.lpstrInitialDir = "M_PosSet_Data";//�����f�B���N�g��

        //�_�C�A���O�{�b�N�X���쐬
        if (type == true) {//����
            if (GetOpenFileNameA(&t) != 0) {
                if (FileName[0]) {//��݂���
                    HANDLE fp;
                    if ((fp = CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
                        if (pos.size() != 0) {
                            if (MessageBox(NULL, "���݂̃f�[�^�ɏ㏑�����܂����H", "M_PosSet_AVIUTL_plugin", MB_OKCANCEL) != IDOK) {
                                return true;
                            }
                        }
                        
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

                        pos.clear();//���ׂĂ̗v�f���폜
                        POS_COUNT = 0;

                        if (n > 0) {
                            int x, y;
                            for (int i = 0; i < n - 1; i++) {
                                if (read_M(t, 22, fp, &dw) == false) {//���l20��(�J���}�����21�o�C�g)�{�u,�v
                                    CloseHandle(fp);//file�����
                                    return true;
                                }
                                else {
                                    Mpos.x = atof(t);
                                }
                                if (read_M(t, 24, fp, &dw) == false) {//���l20��(�J���}�����21�o�C�g)�{�u,�v�{���s�R�[�h
                                    CloseHandle(fp);//file�����
                                    return true;
                                }
                                else {
                                    Mpos.y = atof(t);
                                }

                                Mpos.px = ad(Mpos.x, true);
                                Mpos.py = ad(Mpos.y, false);

                                //���W��ǉ�
                                if (Mpos.px > 0 && Mpos.px < w && Mpos.py > 0 && Mpos.py < h) {
                                    pos.push_back(Mpos);//�V�������W�ǉ�
                                    pos[pos.size() - 1].n = POS_COUNT;
                                    POS_COUNT = POS_COUNT + 1;
                                }
                            }

                            if (read_M(t, 22, fp, &dw) == false) {//���l20��(�J���}�����21�o�C�g)�{�u,�v
                                CloseHandle(fp);//file�����
                                return true;
                            }
                            else {
                                Mpos.x = atof(t);
                            }
                            if (read_M(t, 21, fp, &dw) == false) {//���l20��(�J���}�����21�o�C�g)
                                CloseHandle(fp);//file�����
                                return true;
                            }
                            else {
                                Mpos.y = atof(t);
                            }

                            Mpos.px = ad(Mpos.x, true);
                            Mpos.py = ad(Mpos.y, false);

                            //���W��ǉ�
                            if (Mpos.px > 0 && Mpos.px < w && Mpos.py > 0 && Mpos.py < h) {
                                pos.push_back(Mpos);//�V�������W�ǉ�
                                pos[pos.size() - 1].n = POS_COUNT;
                                POS_COUNT = POS_COUNT + 1;
                            }
                        }

                        CloseHandle(fp);//file�����
                    }
                }
            }
        }
        else {//�o��
            if (GetSaveFileNameA(&t) != 0) {
                if (FileName[0]) {//��������
                    HANDLE fp;

                    if ((fp = CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {//FileName�����邩�ǂ����𒲂ׂ�
                        if (MessageBox(NULL, "�t�@�C�����㏑���ۑ����܂����H", "M_PosSet_AVIUTL_plugin", MB_OKCANCEL) != IDOK) {
                            CloseHandle(fp);//file�����
                            return true;
                        }
                        CloseHandle(fp);//file�����
                    }

                    if ((fp = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
                        //index���Ƀ\�[�g
                        if (pos.size() != 0) {
                            gqsort(0, pos.size() - 1);
                            for (int i = 0; i < pos.size(); i++) {//�ԍ������ߒ���
                                pos[i].n = i;
                            }
                        }
                        POS_COUNT = pos.size();

                        DWORD dw;
                        char t[100];
                        ZeroMemory(&t, 100);
                        int tsize = wsprintfA(t, "n=%10d\r\n", pos.size());
                        if (write_M(t, tsize, fp, &dw) == false) {
                            CloseHandle(fp);//file�����
                            return true;
                        }

                        if (pos.size() == 0) {
                            CloseHandle(fp);//file�����
                            return true;
                        }

                        int a, b, c, d, i;
                        for (i = 0; i < pos.size() - 1; i++) {
                            a = (int)pos[i].x;
                            b = ((pos[i].x - a) * 1000000);
                            if (b < 0) b *= -1;
                            c = (int)pos[i].y;
                            d = ((pos[i].y - c) * 1000000);
                            if (d < 0) d *= -1;
                            tsize = wsprintfA(t, "%14d.%06d,%14d.%06d,\r\n", a, b, c, d);
                            if (write_M(t, tsize, fp, &dw) == false) {
                                CloseHandle(fp);//file�����
                                return true;
                            }
                        }

                        i = pos.size() - 1;
                        a = (int)pos[i].x;
                        b = ((pos[i].x - a) * 1000000);
                        if (b < 0) b *= -1;
                        c = (int)pos[i].y;
                        d = ((pos[i].y - c) * 1000000);
                        if (d < 0) d *= -1;
                        tsize = wsprintfA(t, "%14d.%06d,%14d.%06d\r\n", a, b, c, d);
                        if (write_M(t, tsize, fp, &dw) == false) {
                            CloseHandle(fp);//file�����
                            return true;
                        }

                        CloseHandle(fp);//file�����
                    }
                }
            }
        }

        return true;
    }


    //main�̃E�B���h�E�Ń}�E�X���������Ƃ�
    void mainwind(LPARAM lParam) {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        Mpos.x = (double)x - mainw / 2.0;
        Mpos.y = (double)y - mainh / 2.0;
        Mpos.px = ad(Mpos.x, true);
        Mpos.py = ad(Mpos.y, false);
        Mpos_main = true;
    }

    //index�ԍ��̕\��
    void num_draw(int px, int py, int num) {
        for (int y = py; y < py + 5; y++) {
            for (int x = px; x < px + 4; x++) {
                if (number_data[num][y - py][x - px] == 1) {
                    if (x > 0 && x < w && y > 0 && y < h) {
                        int ix = (h - y) * w + x;
                        data[ix].r = data[ix].r + (255 - data[ix].r) * 0.5;
                        data[ix].g = data[ix].g + (255 - data[ix].g) * 0.5;
                        data[ix].b = data[ix].b + (255 - data[ix].b) * 0.5;
                    }
                }
            }
        }
    }

    void index_draw(int px, int py, int num) {
        if (num < 10000) {
            px = px - 8;
            py = py - 5;
            int n = num / 1000;
            num_draw(px, py, n);//��̈�

            num = num - n * 1000;
            n = num / 100;
            num_draw(px + 4, py, n);//�S�̈�

            num = num - n * 100;
            n = num / 10;
            num_draw(px + 8, py, n);//�\�̈�

            num = num - n * 10;
            n = num / 1;
            num_draw(px + 12, py, n);//��̈�
        }
    }


    //�~�̕`��
    void circle(int posx, int posy) {//�Ƃ肠�����Ă��Ɓ[
        float k = 0;
        int ix = 0;

        for (int y = posy - 3; y < posy + 3; y++) {
            for (int x = posx - 3; x < posx + 3; x++) {
                if (x > 0 && x < w && y > 0 && y < h) {
                    k = zyo2((posx - x), (posy - y));
                    ix = (h - y) * w + x;
                    if (k < 8) {
                        data[ix].r = 252;
                        data[ix].g = 244;
                        data[ix].b = 92;
                    }
                    else if (k < 9) {
                        k = k - 8;
                        data[ix].r = data[ix].r + (252 - data[ix].r) * k;
                        data[ix].g = data[ix].g + (244 - data[ix].g) * k;
                        data[ix].b = data[ix].b + (92 - data[ix].b) * k;
                    }
                }
            }
        }
    }

    //�l�p�̗̈��h��Ԃ�
    void sq(int ax, int ay, int cx, int cy, unsigned char r, unsigned char g, unsigned char b, float a) {
        int ix = 0;
        for (int y = ay; y < cy; y++) {
            for (int x = ax; x < cx; x++) {
                if (x > 0 && x < w && y > 0 && y < h) {
                    ix = (h - y) * w + x;
                    data[ix].r = data[ix].r + (r - data[ix].r) * a;
                    data[ix].g = data[ix].g + (g - data[ix].g) * a;
                    data[ix].b = data[ix].b + (b - data[ix].b) * a;
                }
            }
        }
    }

    //Mpos_main�̂Ƃ�
    void sikaku(POS_XY p) {
        int x = p.px;
        int y = p.py;

        double z = 1.0 / track_zoom;
        x = static_cast<int>(((w - w * z) * 0.5) + x);
        y = static_cast<int>(((h - h * z) * 0.5) + y);

        sq(x - 2, y - 2, x + 2, y + 2, 187, 71, 77, 1.0);
    }

    //�͈͑I���̂Ƃ��̎l�p
    void hani_sikaku(int cx, int cy, int dx, int dy) {
        int sxn, syn, fxn, fyn;

        if (Mpos_ihani == true) {//�͈͈�Ĉړ��̂Ƃ�
            bx = Mpos.px - Mpos_dx;
            by = Mpos.py - Mpos_dy;
            sxn = sx + bx;
            syn = sy + by;
            fxn = fx + bx;
            fyn = fy + by;
        }
        else {
            //�n�_�ƏI�_�����߂�
            sf(dx, dy, cx, cy, &sx, &sy, &fx, &fy);
            sxn = sx;
            syn = sy;
            fxn = fx;
            fyn = fy;
        }

        //�͈͂�h��Ԃ�(63,76,139
        int f = 1;//�g�̕�
        sq(sxn + f, syn + f, fxn - f, fyn - f, 63, 76, 139, 0.4);

        //�g�̕`��
        sq(sxn,     syn,     fxn,    syn + f,63, 76, 139, 0.9);//��
        sq(sxn,     fyn - f, fxn,    fyn,    63, 76, 139, 0.9);//��
        sq(sxn,     syn,     sxn + f,fyn,    63, 76, 139, 0.9);//��
        sq(fxn - f, syn,     fxn,    fyn,    63, 76, 139, 0.9);//�E
    }

    void swap(int* a, int* b) {
        int w = *a;
        *a = *b;
        *b = w;
    }

    //�u���[���n���̐��`��A���S���Y��
    void Bresenham_line(int x1, int y1, int x2, int y2) {

        //��O
        //�P�[�X1�Fx1> x2��(x1,y1)��(x2,y2)��swap
        //�P�[�X2�F���z<0��(x1,-y1)��(x2,-y2)�Ōv�Z���āA(x,-y)�`��
        //�P�[�X3�F���z> 1��(y1,x1)��(y2,x2)�Ōv�Z���āA(y,x)�ŕ`��
        bool r2 = false;//��O��2
        bool r3 = false;//��O��3
        if (x1 != x2) {
            float k = (float)(y2 - y1) / (x2 - x1);//�X��
            if (k < 0) {
                y1 *= -1;
                y2 *= -1;
                k *= -1;
                r2 = true;
            }
            if (k > 1) {
                swap(&x1, &y1);
                swap(&x2, &y2);
                r3 = true;
            }
        }
        else {
            swap(&x1, &y1);
            swap(&x2, &y2);
            r3 = true;
        }
        if (x1 > x2) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }

        int dx = x2 - x1;
        int dy = y2 - y1;
        int d = 2 * dy - dx;
        int E = 2 * dy;
        int NE = 2 * (dy - dx);

        int x = x1, y = y1;
        while (1) {
            if (x == x2) break;

            if (d <= 0) {
                x++;
                d += E;
            }
            else if (d > 0) {
                x++;
                y++;
                d += NE;
            }

            //��O���������ĕ`��
            int px = x;
            int py = y;
            if (r3 == true)
                swap(&px, &py);
            if (r2 == true)
                py *= -1;
            if (px > 0 && px < w && py > 0 && py < h) {
                int ix = (h - py) * w + px;
                data[ix].r = data[ix].r + (255 - data[ix].r) * 0.3;
                data[ix].g = data[ix].g + (254 - data[ix].g) * 0.3;
                data[ix].b = data[ix].b + (102 - data[ix].b) * 0.3;
            }
        }
    }

    //�x�W�F�Ȑ��̊֐�
    void Orbit(float t, float x1, float y1, float x2, float y2, float* x, float* y) {
        float s = 1 - t;
        *x = (3 * s * s * x1 + (3 * s * x2 + t) * t) * t;
        *y = (3 * s * s * y1 + (3 * s * y2 + t) * t) * t;
    }

    void draw_Line(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy) {


        if (ax == dx) {//0���Z���
            dx += 0.001;
        }
        if (ay == dy) {
            dy += 0.001;
        }

        bx = bx - ax;
        cx = cx - ax;
        by = by - ay;
        cy = cy - ay;

        bx = bx / (dx - ax);
        cx = cx / (dx - ax);
        by = by / (dy - ay);
        cy = cy / (dy - ay);

        int n = bezie_seido;//���x

        float x, y;
        Orbit(0, bx, by, cx, cy, &x, &y);
        x = x * (dx - ax) + ax;
        y = y * (dy - ay) + ay;
        x = (int)x;
        y = (int)y;
        if (x > 0 && x < w && y > 0 && y < h) {
            int ix = (h - y) * w + x;
            data[ix].r = 255;
            data[ix].g = 254;
            data[ix].b = 102;
        }

        int vx = x;
        int vy = y;

        for (int i = 1; i < n + 1; i++) {
            Orbit((float)i / n, bx, by, cx, cy, &x, &y);
            x = x * (dx - ax) + ax;
            y = y * (dy - ay) + ay;
            x = (int)x;
            y = (int)y;
            if (x > 0 && x < w && y > 0 && y < h) {
                int ix = (h - y) * w + x;
                data[ix].r = 255;
                data[ix].g = 254;
                data[ix].b = 102;
            }

            Bresenham_line(vx, vy, x, y);
            vx = x;
            vy = y;
        }
    }

    //�摜�f�[�^�ҏW
    bool edit() {
        int ix = 0;

        //�w�i
        for (int i = 0; i < w * h; i++) {
            data[i].r = 80;
            data[i].g = 80;
            data[i].b = 80;
        }

        //����
        int s = h % 25 - 2;
        if (s < 0) s = s + 25;
        for (int y = s; y < h; y = y + 25) {
            for (int i = 0; i < 2; i++) {
                ix = (y + i) * w;
                for (int x = 0; x < w; x++) {
                    data[ix].r = 100;
                    data[ix].g = 100;
                    data[ix].b = 100;
                    ix++;
                }
            }
        }

        //�c��
        for (int x = 0; x < w - 1; x = x + 25) {
            for (int i = 0; i < 2; i++) {
                ix = x + i;
                for (int y = 0; y < h; y++) {
                    data[ix].r = 100;
                    data[ix].g = 100;
                    data[ix].b = 100;
                    ix = ix + w;
                }
            }
        }

        //�g�\��(0,200,200
        if (flag_draw_waku == true) {
            if (track_zoom >= 1) {
                double z = 1.0 / track_zoom;
                int sxn = static_cast<int>((w - w * z) * 0.5);
                int syn = static_cast<int>((h - h * z) * 0.5);
                int fxn = w - sxn + 1;
                int fyn = h - syn + 1;

                int f = 1;
                sq(sxn, syn, fxn, syn + f, 0, 200, 200, 0.3);//��
                sq(sxn, fyn - f, fxn, fyn, 0, 200, 200, 0.3);//��
                sq(sxn, syn, sxn + f, fyn, 0, 200, 200, 0.3);//��
                sq(fxn - f, syn, fxn, fyn, 0, 200, 200, 0.3);//�E
            }
        }

        //�x�W�F�Ȑ��̕`�掞�̑O����
        if (pos.size() > 3 && flag_draw_bezie == true) {
            //index���Ƀ\�[�g
            gqsort(0, pos.size() - 1);
            for (int i = 0; i < pos.size(); i++) {
                pos[i].n = i;
            }
            POS_COUNT = pos.size();

            //�}�E�X���W�Ɠ���̍��W��T��
            if (Mpos_umu == true) {
                for (int i = 0; i < pos.size(); i++) {
                    if (Mpos.px == pos[i].px && Mpos.py == pos[i].py) {
                        if (Mpos.x == pos[i].x && Mpos.y == pos[i].y) {
                            //�������W����������A�}�E�X�̔ԍ�����������
                            Mpos.n = i;
                            break;
                        }
                    }
                }
            }
        }
        
        if (pos.size() != 0) {
            //���W��pos_data_for_draw�ɂ܂Ƃߒ���
            float* p = NULL;
            if (pos.size() != pos_data_for_draw_num) {
                delete[] pos_data_for_draw;
                p = new(std::nothrow)float[pos.size() * 2];//�����������ف[
                if (p == NULL) {
                    pos_data_for_draw = nullptr;
                    return false;
                }
                else {
                    pos_data_for_draw = p;
                    pos_data_for_draw_num = pos.size();
                }
            }
            else {
                p = pos_data_for_draw;
                if (p == nullptr)
                    return false;
            }

            //�������ɏ�������ł���
            if (Mpos_ihani == true) {//�͈͈�Ĉړ��̂Ƃ�
                bx = Mpos.px - Mpos_dx;
                by = Mpos.py - Mpos_dy;
                for (int i = 0; i < pos.size(); i++) {
                    if (pos[i].px >= sx && pos[i].px <= fx && pos[i].py >= sy && pos[i].py <= fy) {
                        p[i * 2] = pos[i].px + bx;
                        p[i * 2 + 1] = pos[i].py + by;
                    }
                    else {
                        p[i * 2] = pos[i].px;
                        p[i * 2 + 1] = pos[i].py;
                    }
                }
            }
            else {
                for (int i = 0; i < pos.size(); i++) {
                    p[i * 2] = pos[i].px;
                    p[i * 2 + 1] = pos[i].py;
                }
            }

            //�x�W�F�Ȑ��̕`��(�֑���(255,254,102
            if (pos.size() > 3 && flag_draw_bezie == true) {
                int c = 0;
                while (1) {
                    if (c + 3 > pos.size() - 1) break;
                    draw_Line(*(p + c * 2), *(p + c * 2 + 1),
                        *(p + (c + 1) * 2), *(p + (c + 1) * 2 + 1),
                        *(p + (c + 2) * 2), *(p + (c + 2) * 2 + 1),
                        *(p + (c + 3) * 2), *(p + (c + 3) * 2 + 1));
                    c = c + 3;
                }

                if (c + 2 == pos.size() - 1) {//���������
                    draw_Line(*(p + c * 2), *(p + c * 2 + 1),
                        *(p + (c + 1) * 2), *(p + (c + 1) * 2 + 1),
                        *(p + (c + 2) * 2), *(p + (c + 2) * 2 + 1),
                        *(p + 0), *(p + 1));
                }
            }

            //�����`��
            if (flag_draw_line == true) {
                int x = *(p + 0);
                int y = *(p + 1);
                for (int i = 1; i < pos.size(); i++) {
                    Bresenham_line(x, y, *(p + i * 2), *(p + i * 2 + 1));
                    x = *(p + i * 2);
                    y = *(p + i * 2 + 1);
                }
            }

            //���W�`��
            for (int i = 0; i < pos.size(); i++) {
                circle((int)p[i * 2], (int)p[i * 2 + 1]);
                if (flag_draw_number == true)
                    index_draw((int)p[i * 2], (int)p[i * 2 + 1], pos[i].n);
            }
        }
        else {
            delete[] pos_data_for_draw;
            pos_data_for_draw = nullptr;
            pos_data_for_draw_num = 0;
        }

        //�͈͑I��
        if (Mpos_hani == true) {
            if (Mpos_shani == true)
                hani_sikaku(Mpos_dx, Mpos_dy, Mpos_sx, Mpos_sy);
            else
                hani_sikaku(Mpos_dx, Mpos_dy, Mpos.px, Mpos.py);
        }

        //�}�E�X���W
        if (Mpos_main) {
            sikaku(Mpos);
        }

        //���o�̓{�^��(195,61,185),(81,187,71)
        if (flag_draw_button == true) {
            sq(10, 10, 16, 16, 195, 61, 185, 1.0);
            sq(10, 20, 16, 26, 81, 187, 71, 1.0);
        }
        
        return true;
    }


    //�`��
    bool t(FILTER* fp, void* editp, LPARAM lParam)
    {
        bool ach = wh(fp, lParam);
        if (ach == false) return false;

        if (fp == NULL) return false;
        HDC hdc0;
        static int editing = FALSE;

        hdc0 = GetDC(fp->hwnd);

        if (fp) editing = fp->exfunc->is_editing(editp);
        if (data && editing) {
            edit();
            //�\��t��
            SetDIBitsToDevice(hdc0, 0, 0, w, h, 0, 0, 0, h, data, &d, DIB_RGB_COLORS);
        }
        else {
            //MessageBox(NULL, "�Ȃ�ŃG���[�Ȃ��c", "a", MB_OK);
        }

        ReleaseDC(fp->hwnd, hdc0);

        
        if (flag_draw_alpha == true) {
            // �����ʂ�X�^�C����t��������
            long lStyle = GetWindowLong(fp->hwnd, GWL_EXSTYLE) | 0x00080000;
            SetWindowLong(fp->hwnd, GWL_EXSTYLE, lStyle);
            if (alpha_atai != alpha_atai_d) {
                SetLayeredWindowAttributes(fp->hwnd, 0, alpha_atai, LWA_ALPHA);
                alpha_atai_d = alpha_atai;
            }
        }
        else if (alpha_atai_d != 255) {
            SetLayeredWindowAttributes(fp->hwnd, 0, 255, LWA_ALPHA);
            alpha_atai_d = 255;
        }

        return true;
    }


    //�\�[�g�֐�
    void gqsort(int f, int l) {
        int i = f, j = l;
        int pivot = pos[(f + l) / 2].n;

        while (1) {
            while (pos[i].n < pivot)
                i++;
            while (pos[j].n > pivot)
                j--;
            if (i >= j) break;

            POS_XY n = pos[i];//swap
            pos[i] = pos[j];
            pos[j] = n;

            i++;
            j--;
        }

        if (f < i - 1) gqsort(f, i - 1);
        if (j + 1 < l) gqsort(j + 1, l);
    }


    //���W���|�C���^�ɏ�������
    int Pwrite(bool c, void* p) {
        if (c == true) {//�v�f����int�ŏ�������
            int* a = reinterpret_cast<int*>(p);
            *a = pos.size();
            return *a;
        }
        else {//���W(x,y)����������
            //index���Ƀ\�[�g
            if (pos.size() != 0) {
                gqsort(0, pos.size() - 1);
                for (int i = 0; i < pos.size(); i++) {//�ԍ������ߒ���
                    pos[i].n = i;
                }
            }
            POS_COUNT = pos.size();

            //��������
            double* a = reinterpret_cast<double*>(p);
            for (int i = 0; i < pos.size(); i++) {
                a[i * 2] = pos[i].x;
                a[i * 2 + 1] = pos[i].y;
            }
            return 0;
        }
    }


    //�������̉���Ƃ��A�ϐ��Ƃ�
    void FreeMemoryData() {
        delete[] data;
        data = nullptr;
        dw = 0;
        dh = 0;

        delete[] pos_data_for_draw;
        pos_data_for_draw = nullptr;
        pos_data_for_draw_num = 0;
    }

    windraw() {
        bool b[][5][4] = {
            {//0
            {0,1,1,1},
            {0,1,0,1},
            {0,1,0,1},
            {0,1,0,1},
            {0,1,1,1}
            },
            {//1
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0}
            },
            {//2
            {0,1,1,1},
            {0,0,0,1},
            {0,1,1,1},
            {0,1,0,0},
            {0,1,1,1}
            },
            {//3
            {0,1,1,1},
            {0,0,0,1},
            {0,1,1,1},
            {0,0,0,1},
            {0,1,1,1}
            },
            {//4
            {0,1,0,1},
            {0,1,0,1},
            {0,1,1,1},
            {0,0,0,1},
            {0,0,0,1}
            },
            {//5
            {0,1,1,1},
            {0,1,0,0},
            {0,1,1,1},
            {0,0,0,1},
            {0,1,1,1}
            },
            {//6
            {0,1,0,0},
            {0,1,0,0},
            {0,1,1,1},
            {0,1,0,1},
            {0,1,1,1}
            },
            {//7
            {0,1,1,1},
            {0,1,0,1},
            {0,0,0,1},
            {0,0,0,1},
            {0,0,0,1}
            },
            {//8
            {0,1,1,1},
            {0,1,0,1},
            {0,1,1,1},
            {0,1,0,1},
            {0,1,1,1}
            },
            {//9
            {0,1,1,1},
            {0,1,0,1},
            {0,1,1,1},
            {0,0,0,1},
            {0,0,0,1}
            },
        };

        //�R�s�[
        for (int i = 0; i < 10; i++) {
            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 4; x++) {
                    number_data[i][y][x] = b[i][y][x];
                }
            }
        }
    }

    ~windraw() {
        FreeMemoryData();
    }


};