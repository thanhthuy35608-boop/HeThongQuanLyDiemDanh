#include "Module5_MenuController.h"
#include "Module2_DataManager.h"

int main() {
    // Kích hoạt toàn bộ luồng chương trình
    int capacityLop = 50;// Sức chứa ban đầu cho mảng lớp học
    LopHoc** dsLop = new LopHoc*[capacityLop];
    int soLop = 0;//

    // Đọc dữ liệu đầu vào từ thư mục data
    docDuLieuTu3FileTXT(dsLop, soLop, capacityLop);
    hienThiMenuChinh(dsLop, soLop, capacityLop);
    
    // Giải phóng bộ nhớ
    for (int i = 0; i < soLop; i++) {
        delete dsLop[i];
    }
    delete[] dsLop;
    
    return 0;
}