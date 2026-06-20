#ifndef MODULE1_MODELS_H
#define MODULE1_MODELS_H


#include <string>
using namespace std;


struct TietHoc {
    int thu;
    string tiet;
    string phong;
    string monHoc;
};


struct NgayDiemDanh {
    string date;
    int trangThai; // 1: Co mat, 2: Vang khong phep, 3: Co phep
};


struct SinhVien {
    int STT;
    string MSSV, hoTen, ngaySinh;
    NgayDiemDanh* dsDiemDanh;
    int tongSoBuoi, capacity;


    SinhVien();
    SinhVien(int stt, string mssv, string ten, string ngSinh);
    ~SinhVien();
    SinhVien(const SinhVien& other);
    SinhVien& operator=(const SinhVien& other);
};


struct NodeSinhVien {
    SinhVien data;
    NodeSinhVien* next;
    NodeSinhVien(SinhVien sv);
};


struct LopHoc {
    string maLop, tenLop;
    TietHoc* dsTiet;
    int soTiet, capacityTiet;
    NodeSinhVien* head;
    int siSo;


    LopHoc();
    ~LopHoc();
    LopHoc(const LopHoc&) = delete;
    LopHoc& operator=(const LopHoc&) = delete;
};


// HAM HO TRO
void themTietHoc(LopHoc& lop, int thu, string tiet, string phong, string monHoc);
void themNgayDiemDanh(SinhVien& sv, string ngay, int trangThai);
void themSinhVienVaoLop(LopHoc& lop, const SinhVien& sv);


#endif


