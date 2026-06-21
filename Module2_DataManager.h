#ifndef MODULE2_DATAMANAGER_H
#define MODULE2_DATAMANAGER_H


#include "Module1_Models.h"
#include <string>
#include <fstream>


using namespace std;


// --- CÁC HÀM TIỆN ÍCH XỬ LÝ CHUỖI ---
bool moFileNhap(ifstream& fin, const string paths[], int len);
void tachChuoi(const string& s, char delim, string* parts, int& count, int maxParts);

// --- CÁC HÀM THAO TÁC DỮ LIỆU CƠ BẢN (THÊM / XÓA) ---
int timChiSoLop(LopHoc** dsLop, int soLop, string maLop);
LopHoc* chonLop(LopHoc** dsLop, int soLop);

// --- CÁC HÀM ĐỌC / GHI FILE ---
void docDuLieuTu3FileTXT(LopHoc**& dsLop, int& soLop, int& capacityLop);
void luuDuLieuDiemDanh(LopHoc** dsLop, int soLop, string tenFile);

#endif 


