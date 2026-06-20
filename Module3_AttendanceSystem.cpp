#include "Module3_AttendanceSystem.h"
#include "Module2_DataManager.h"
#include <iostream>


using namespace std;


string chuanHoaTrangThai(string nhap) {
    for (char &c : nhap) c = toupper(c);
    if (nhap == "C" || nhap == "CO MAT") return "Co mat";
    if (nhap == "V" || nhap == "VANG") return "Vang khong phep";
    if (nhap == "CP" || nhap == "CO PHEP") return "Vang co phep";
    return "";
}


int trangThaiToCode(string trangThai) {
    if (trangThai == "Co mat") return 1;
    if (trangThai == "Vang khong phep") return 2;
    if (trangThai == "Vang co phep") return 3;
    return 0;
}


string codeToTrangThai(int code) {
    if (code == 1) return "Co mat";
    if (code == 2) return "Vang khong phep";
    if (code == 3) return "Vang co phep";
    return "Chua diem danh";
}


// Luồng điểm danh đồng thời cho cả lớp (Duyệt DSLK)
void thucHienDiemDanh(LopHoc** dsLop, int soLop, string maLop, string ngayHoc) {
    int idx = timChiSoLop(dsLop, soLop, maLop);
    if (idx == -1) {
        cout << "Loi: Ma lop hoc '" << maLop << "' khong ton tai tren he thong!\n";
        return;
    }
    LopHoc* lopSelected = dsLop[idx];
    if (lopSelected->siSo == 0) {
        cout << "Lop hoc '" << lopSelected->tenLop << "' chua co sinh vien nao de diem danh!\n";
        return;
    }

    cout << "\n--- DIEM DANH LOP: " << lopSelected->tenLop << " | NGAY: " << ngayHoc << " ---\n";
    cout << "Quy uoc nhap: C (Co mat), V (Vang khong phep), CP (Co phep)\n";


    NodeSinhVien* curr = lopSelected->head;
    int stt = 1;
    while (curr != nullptr) {
        SinhVien& sv = curr->data;
        string nhapTrangThai = "";
        string trangThaiHopLe = "";


        while (trangThaiHopLe == "") {
            cout << "STT: " << stt <<"|"<< sv.hoTen << " (" << sv.MSSV << "): ";
            cin >> nhapTrangThai;
            trangThaiHopLe = chuanHoaTrangThai(nhapTrangThai);
            if (trangThaiHopLe == "") {
                cout << "-> Ky tu nhap khong hop le! Vui long nhap lai (C / V / CP).\n";
            }
        }
        stt ++;

        int codeTrangThai = trangThaiToCode(trangThaiHopLe);
        bool daGhiDe = false;


        // Thuật toán ghi đè trùng ngày trực tiếp trên mảng động của SV
        for (int i = 0; i < sv.tongSoBuoi; i++) {
            if (sv.dsDiemDanh[i].date == ngayHoc) {
                sv.dsDiemDanh[i].trangThai = codeTrangThai;
                daGhiDe = true;
                break;
            }
        }


        if (!daGhiDe) {
            themNgayDiemDanh(sv, ngayHoc, codeTrangThai);
        }
        curr = curr->next;
    }
    cout << "==> Da ghi nhan diem danh thanh cong vao bo nho he thong!\n";
    luuDuLieuDiemDanh (dsLop, soLop, "attendance_records.txt"); // Lưu ngay sau khi điểm danh
}


// Thuật toán tính tỷ lệ vắng mặt và đưa ra cảnh báo cấm thi (>20%)
void tinhTyLeVangVaCanhBao(LopHoc** dsLop, int soLop, string maLop) {
    int idx = timChiSoLop(dsLop, soLop, maLop);
    if (idx == -1) return;
    LopHoc* lopSelected = dsLop[idx];


    cout << "\n=======================================================\n";
    cout << "   BANG THONG KE TI LE VANG MAT LOP: " << lopSelected->tenLop << "\n";
    cout << "=======================================================\n";


    NodeSinhVien* curr = lopSelected->head;
    while (curr != nullptr) {
        SinhVien& sv = curr->data;
        int tongSoBuoiDaDiemDanh = sv.tongSoBuoi;
        int soBuoiVangMat = 0;


        for (int i = 0; i < sv.tongSoBuoi; i++) {
            if (sv.dsDiemDanh[i].trangThai == 2 || sv.dsDiemDanh[i].trangThai == 3) {
                soBuoiVangMat++;
            }
        }

        if (tongSoBuoiDaDiemDanh > 0) {
            float tyLeVang = ((float)soBuoiVangMat / tongSoBuoiDaDiemDanh) * 100;
            if (tyLeVang > 20.0) {
                cout << "MSSV: " << sv.MSSV << " | " << sv.hoTen << " | Vang: " << soBuoiVangMat << " buoi, chiem "<< tyLeVang << "%";
                cout << " -> [CANH BAO: NGUY CO CAM THI!]";
                cout << "\n";
            }
        }
        curr = curr->next;
    }
    cout << "=======================================================\n";
}

void capNhatTrangThaiDiemDanh(LopHoc** dsLop, int soLop, string maLop, string ngayHoc, string mssv, string trangThaiMoi) {
    int idx = timChiSoLop(dsLop, soLop, maLop);
    if (idx == -1) {
        cout << "Ma lop khong ton tai!" << endl;
        return;
    }
    int trangThai = trangThaiToCode(chuanHoaTrangThai(trangThaiMoi));
    if (trangThai == 0) {
        cout << "Trang thai diem danh khong hop le!" << endl;
        return;
    }
    LopHoc* lopSelected = dsLop[idx];
    NodeSinhVien* curr = lopSelected->head;
    bool found = false;
    while (curr != nullptr) {
        if (curr->data.MSSV == mssv) {
            found = true;
            bool updated = false;
            for (int i = 0; i < curr->data.tongSoBuoi; i++) {
                if (curr->data.dsDiemDanh[i].date == ngayHoc) {
                    curr->data.dsDiemDanh[i].trangThai = trangThai;
                    updated = true;
                    break;
                }
            }
            if (!updated) {
                themNgayDiemDanh(curr->data, ngayHoc, trangThai);
            }
            cout << "Da cap nhat diem danh cho MSSV " << mssv << " ngay " << ngayHoc << ".\n";
            luuDuLieuDiemDanh (dsLop, soLop, "attendance_records.txt"); // Lưu ngay sau khi cập nhật
            break;
        }
        curr = curr->next;
    }
    if (!found) {
        cout << "Khong tim thay sinh vien MSSV " << mssv << " trong lop " << maLop << ".\n";
    }
}