#ifndef MODULE4_SEARCHSERVICE_H
#define MODULE4_SEARCHSERVICE_H


#include "Module1_Models.h"
#include <string>


using namespace std;


// Các hàm tìm kiếm nâng cao
void timKiemDiemDanhTheoSinhVien(LopHoc** dsLop, int soLop, string maSV);
void xemThongTinLop(LopHoc** dsLop, int soLop, string maLop);

// Các hàm xuất báo cáo thống kê chuyên sâu
void timKiemDiemDanhTheoBuoi(LopHoc** dsLop, int soLop, string maLop, string ngayHoc);
void inDanhSachSinhVienVangNhieuNhat(LopHoc** dsLop, int soLop, string maLop);


#endif
