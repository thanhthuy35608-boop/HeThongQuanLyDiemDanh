#ifndef MODULE3_ATTENDANCESYSTEM_H
#define MODULE3_ATTENDANCESYSTEM_H


#include "Module1_Models.h"
#include <string>


using namespace std;

// Luong nghiep vu diem danh cot loi
void thucHienDiemDanh(LopHoc** dsLop, int soLop, string maLop, string ngayHoc);
void tinhTyLeVangVaCanhBao(LopHoc** dsLop, int soLop, string maLop);
void capNhatTrangThaiDiemDanh(LopHoc** dsLop, int soLop, string maLop, string ngayHoc, string maSV, string trangThaiMoi);

// Ham tien ich ho tro chuan hoa va chuyen doi trang thai
string chuanHoaTrangThai(string nhap);
int trangThaiToCode(string trangThai);
string codeToTrangThai(int code);


#endif // MODULE3_ATTENDANCESYSTEM_H
