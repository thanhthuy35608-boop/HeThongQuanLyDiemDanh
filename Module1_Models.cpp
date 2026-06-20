#include "Module1_Models.h"
#include <iostream>
using namespace std;


// SINH VIEN

SinhVien::SinhVien() : STT(0), MSSV(""), hoTen(""), ngaySinh(""), tongSoBuoi(0), capacity(5) {
    dsDiemDanh = new NgayDiemDanh[capacity];
}


SinhVien::SinhVien(int stt, string mssv, string ten, string ngSinh)
    : STT(stt), MSSV(mssv), hoTen(ten), ngaySinh(ngSinh), tongSoBuoi(0), capacity(5) {
    dsDiemDanh = new NgayDiemDanh[capacity];
}


SinhVien::~SinhVien() {
    delete[] dsDiemDanh;
}


SinhVien::SinhVien(const SinhVien& other) {
    STT = other.STT;
    MSSV = other.MSSV;
    hoTen = other.hoTen;
    ngaySinh = other.ngaySinh;
    tongSoBuoi = other.tongSoBuoi;
    capacity = other.capacity;
    dsDiemDanh = new NgayDiemDanh[capacity];
    for (int i = 0; i < tongSoBuoi; i++) {
        dsDiemDanh[i] = other.dsDiemDanh[i];
    }
}


SinhVien& SinhVien::operator=(const SinhVien& other) {
    if (this != &other) {
        delete[] dsDiemDanh;
        STT = other.STT;
        MSSV = other.MSSV;
        hoTen = other.hoTen;
        ngaySinh = other.ngaySinh;
        tongSoBuoi = other.tongSoBuoi;
        capacity = other.capacity;
        dsDiemDanh = new NgayDiemDanh[capacity];
        for (int i = 0; i < tongSoBuoi; i++) {
            dsDiemDanh[i] = other.dsDiemDanh[i];
        }
    }
    return *this;
}


NodeSinhVien::NodeSinhVien(SinhVien sv) : data(sv), next(NULL) {}


// LOP HOC

LopHoc::LopHoc() : maLop(""), tenLop(""), soTiet(0), capacityTiet(4), head(NULL), siSo(0) {
    dsTiet = new TietHoc[capacityTiet];
}


LopHoc::~LopHoc() {
    delete[] dsTiet;
    NodeSinhVien* curr = head;
    while (curr) {
        NodeSinhVien* next = curr->next;
        delete curr;
        curr = next;
    }
}


// HAM HO TRO

void themTietHoc(LopHoc& lop, int thu, string tiet, string phong, string monHoc) {
    if (lop.soTiet >= lop.capacityTiet) {
        int newCapacity = (lop.capacityTiet == 0) ? 2 : lop.capacityTiet * 2;
        TietHoc* newDsTiet = new TietHoc[newCapacity];
        for (int i = 0; i < lop.soTiet; i++) {
            newDsTiet[i] = lop.dsTiet[i];
        }
        delete[] lop.dsTiet;
        lop.dsTiet = newDsTiet;
        lop.capacityTiet = newCapacity;
    }
    lop.dsTiet[lop.soTiet].thu = thu;
    lop.dsTiet[lop.soTiet].tiet = tiet;
    lop.dsTiet[lop.soTiet].phong = phong;
    lop.dsTiet[lop.soTiet].monHoc = monHoc;
    lop.soTiet++;
}


void themNgayDiemDanh(SinhVien& sv, string ngay, int trangThai) {
    // Kiem tra da co ngay nay chua
    for (int i = 0; i < sv.tongSoBuoi; i++) {
        if (sv.dsDiemDanh[i].date == ngay) {
            sv.dsDiemDanh[i].trangThai = trangThai;
            return;
        }
    }
   
    // Mo rong mang neu can
    if (sv.tongSoBuoi >= sv.capacity) {
        int newCap = sv.capacity * 2;
        NgayDiemDanh* newArr = new NgayDiemDanh[newCap];
        for (int i = 0; i < sv.tongSoBuoi; i++) {
            newArr[i] = sv.dsDiemDanh[i];
        }
        delete[] sv.dsDiemDanh;
        sv.dsDiemDanh = newArr;
        sv.capacity = newCap;
    }
   
    sv.dsDiemDanh[sv.tongSoBuoi].date = ngay;
    sv.dsDiemDanh[sv.tongSoBuoi].trangThai = trangThai;
    sv.tongSoBuoi++;
}

void themSinhVienVaoLop(LopHoc& lop, const SinhVien& sv) {
    NodeSinhVien* newNode = new NodeSinhVien(sv);
    if (!lop.head) {
        lop.head = newNode;
    } else {
        NodeSinhVien* curr = lop.head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newNode;
    }
    lop.siSo++;
}