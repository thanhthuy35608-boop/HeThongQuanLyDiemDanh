#include "Module4_SearchService.h"
#include "Module3_AttendanceSystem.h"
#include "Module2_DataManager.h"
#include <iostream>


using namespace std;

//Tìm kiếm thông tin điểm danh của 1 lớp học vào 1 ngày cụ thể.
void timKiemDiemDanhTheoNgay(LopHoc** dsLop, int soLop, string maLop, string ngayHoc) {
    int idx = timChiSoLop(dsLop, soLop, maLop);
    if (idx == -1) {
        cout << "Loi: Khong tim thay ma lop " << maLop << "\n";
        return;
    }
    LopHoc* lopSelected = dsLop[idx];

    cout << "\n--- KET QUA TIM KIEM DIEM DANH LOP: " << maLop << " | NGAY: " << ngayHoc << " ---\n";
    bool coDuLieu = false;

    NodeSinhVien* curr = lopSelected->head;
    int stt = 1;
    while (curr != nullptr) {
        SinhVien& sv = curr->data;
        for (int i = 0; i < sv.tongSoBuoi; i++) {
            if (sv.dsDiemDanh[i].date == ngayHoc) {
                cout << "STT: "<< stt <<" |MSSV: " << sv.MSSV << " | Ho ten: " << sv.hoTen
                     << " | Trang thai: " << codeToTrangThai(sv.dsDiemDanh[i].trangThai) << "\n";
                coDuLieu = true;
                break;
            }
        }
        curr = curr->next;
        stt ++;
    }


    if (!coDuLieu) {
        cout << "-> Khong tim thay du lieu diem danh cho ngay hoc nay.\n";
    }
    cout << "---------------------------------------------------------\n";
}

// Tìm kiếm lịch sử đi học của một sinh viên bất kỳ qua tất cả các lớp
void timKiemDiemDanhTheoSinhVien(LopHoc** dsLop, int soLop, string maSV) {
    bool timThaySV = false;


    for (int k = 0; k < soLop; k++) {
        LopHoc* lop = dsLop[k];
        NodeSinhVien* curr = lop->head;
       
        while (curr != nullptr) {
            if (curr->data.MSSV == maSV) {
                if (!timThaySV) {
                    cout << "\n--- LICH SU DIEM DANH CUA SV: " << curr->data.hoTen << " (" << maSV << ") ---\n";
                    timThaySV = true;
                }
                cout << "Tại lop: " << lop->maLop << " (" << lop->tenLop << "):\n";
                if (curr->data.tongSoBuoi == 0) {
                    cout << "   -> Chua co du lieu ghi nhan o lop nay.\n";
                } else {
                    for (int i = 0; i < curr->data.tongSoBuoi; i++) {
                        cout << "   + Ngay: " << curr->data.dsDiemDanh[i].date
                             << " | Trang thai: " << codeToTrangThai(curr->data.dsDiemDanh[i].trangThai) << "\n";
                    }
                }
            }
            curr = curr->next;
        }
    }


    if (!timThaySV) {
        cout << "-> Khong tim thay sinh vien co ma " << maSV << " trong he thong.\n";
    }
    cout << "---------------------------------------------------------\n";
}


// Xuất danh sách kỷ lục những sinh viên vắng nhiều buổi học nhất lớp
void inDanhSachSinhVienVangNhieuNhat(LopHoc** dsLop, int soLop, string maLop) {
    int idx = timChiSoLop(dsLop, soLop, maLop);
    if (idx == -1) return;
    LopHoc* lopSelected = dsLop[idx];

    if (lopSelected->siSo == 0) {
        cout << "Lop hoc nay khong co sinh vien.\n";
        return;
    }

    struct KetQuaSV {
        string MSSV;
        string hoTen;
        int soBuoiVang;
    };

    int capKetQua = 16;
    int soKetQua = 0;
    KetQuaSV* danhSach = new KetQuaSV[capKetQua];
    NodeSinhVien* curr = lopSelected->head;
    while (curr != nullptr) {
        int soBuoiVang = 0;
        for (int i = 0; i < curr->data.tongSoBuoi; i++) {
            if (curr->data.dsDiemDanh[i].trangThai == 2 || curr->data.dsDiemDanh[i].trangThai == 3) {
                soBuoiVang++;
            }
        }
        if (soKetQua >= capKetQua) {
            int newCap = capKetQua * 2;
            KetQuaSV* temp = new KetQuaSV[newCap];
            for (int i = 0; i < soKetQua; i++) {
                temp[i] = danhSach[i];
            }
            delete[] danhSach;
            danhSach = temp;
            capKetQua = newCap;
        }
        danhSach[soKetQua].MSSV = curr->data.MSSV;
        danhSach[soKetQua].hoTen = curr->data.hoTen;
        danhSach[soKetQua].soBuoiVang = soBuoiVang;
        soKetQua++;
        curr = curr->next;
    }

    cout << "\n=======================================================\n";
    cout << "   DANH SACH SINH VIEN CO BUOI NGHI NHIEU NHAT LOP: " << lopSelected->tenLop << "\n";
    cout << "=======================================================\n";

    int maxVang = soKetQua == 0 ? 0 : danhSach[0].soBuoiVang;
    if (maxVang == 0) {
        cout << " -> Tuyet voi! Chua co sinh vien nao vang buoi nao.\n"; return;
    }
    for (int i = 0; i < soKetQua; i++) {
        if (danhSach[i].soBuoiVang == maxVang) {
            cout << "MSSV: " << danhSach[i].MSSV << " | Ho ten: " << danhSach[i].hoTen
                 << " | So buoi vang: " << danhSach[i].soBuoiVang << "\n";
        }
    }

    delete[] danhSach;
    cout << "=======================================================\n";
}

//Xem toàn bộ thông tin của lớp học bao gồm mã lớp, tên môn học, phòng học, danh sách sinh viên thuộc lớp.
void xemThongTinLop(LopHoc** dsLop, int soLop, string maLop) {
    int idx = timChiSoLop(dsLop, soLop, maLop);
    if (idx == -1) {
        cout << "Khong tim thay lop hoc co ma: " << maLop << "\n";
        return;
    }
    LopHoc* lopSelected = dsLop[idx];

    cout << "\n--- THONG TIN LOP HOC: " << lopSelected->tenLop << " (" << lopSelected->maLop << ") ---\n";
    cout << "So tiet hoc (buoi hoc): " << lopSelected->soTiet << "\n";
    cout << "Danh sach tiet hoc:\n";
    for (int i = 0; i < lopSelected->soTiet; i++) {
        cout << "  + Thu: " << lopSelected->dsTiet[i].thu
             << ", Tiet: " << lopSelected->dsTiet[i].tiet
             << ", Phong: " << lopSelected->dsTiet[i].phong
             << ", Mon hoc: " << lopSelected->tenLop << "\n";
    }

    cout << "Danh sach sinh vien:\n";
    NodeSinhVien* curr = lopSelected->head; 
    int stt = 1;
    while (curr != nullptr) {
        cout <<"STT: "<< stt << ", MSSV: " << curr->data.MSSV
             << ", Ho ten: " << curr->data.hoTen
             << ", Ngay sinh: " << curr->data.ngaySinh
             << "\n";
        curr = curr->next;
        stt ++;
    }
}

