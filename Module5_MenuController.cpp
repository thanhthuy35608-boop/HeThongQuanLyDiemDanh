#include "Module2_DataManager.h"
#include "Module3_AttendanceSystem.h"
#include "Module4_SearchService.h"
#include <iostream>
#include <string>


using namespace std;


void hienThiMenuChinh(LopHoc**& dsLop, int& soLop, int& capacityLop) {
    int choice = -1;
    string maLop, ngayHoc, maSV;
    do{
        cout << "\n=======================================================\n";
        cout << "       HE THONG QUAN LY DIEM DANH SINH VIEN              \n";
        cout << "=======================================================\n";
        cout << " [1] Xem danh sach lop & thoi khoa bieu\n";
        cout << " [2] Canh bao cam thi theo lop \n"; 
        cout << " [3] Tien hanh Diem danh lop hoc\n";
        cout << " [4] Chinh sua / Cap nhat trang thai diem danh\n";
        cout << " [5] Tim kiem diem danh theo ma sinh vien\n";//
        cout << " [6] Tim kiem diem danh theo ngay cua lop hoc\n";
        cout << " [7] Danh sach sinh vien vang nhieu nhat\n"; 
        cout << " [0] Luu tat ca du lieu va Thoat chuong trinh\n";
        cout << "=======================================================\n";
        cout << " Nhap lua chon cua ban (0-7): ";
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Lua chon khong hop le!\n";
            continue ;
        }
        cin.ignore();

        switch (choice) {
            case 1: {
                cout << "\nNhap ma lop de xem danh sach sinh vien va thoi khoa bieu: ";
                getline(cin, maLop);
                xemThongTinLop(dsLop, soLop, maLop);
                break;
            }

            case 2: {
                cout << "\nNhap ma lop : ";
                getline(cin, maLop);
                if (timChiSoLop(dsLop, soLop, maLop) == -1) {
                    cout << "Ma lop khong ton tai!\n";
                    break;
                }
                tinhTyLeVangVaCanhBao(dsLop, soLop, maLop);
                break;
            }

           
            case 3: {
                if (soLop == 0) { cout << "Khong co lop hoc nao!\n"; break; }
                cout << "Nhap ma lop: "; getline(cin, maLop);
                if (timChiSoLop(dsLop, soLop, maLop) == -1) { 
                    cout << "Ma lop khong ton tai!\n"; break; 
                }
                cout << "Nhap ngay hoc (dd/mm/yyyy): "; getline(cin, ngayHoc);
                thucHienDiemDanh(dsLop, soLop, maLop, ngayHoc);
                break;
            }

            case 4: {
                if (soLop == 0) { cout << "Khong co lop hoc nao!\n"; break; }
                cout << "Nhap ma lop: "; getline(cin, maLop);
                cout << "Nhap ngay hoc (dd/mm/yyyy): "; getline(cin, ngayHoc);
                cout << "Nhap ma sinh vien: "; getline(cin, maSV);
                string trangThaiMoi;
                cout << "Nhap trang thai moi (C/V/CP): "; getline(cin, trangThaiMoi);
                capNhatTrangThaiDiemDanh(dsLop, soLop, maLop, ngayHoc, maSV, trangThaiMoi);
                break;
            }

            case 5: {
                if (soLop == 0) { cout << "Khong co du lieu!\n"; break; }
                cout << "Nhap ma sinh vien: "; getline(cin, maSV);
                timKiemDiemDanhTheoSinhVien(dsLop, soLop, maSV);
                break;
            }

            case 6: {
                if (soLop == 0) { cout << "Khong co lop hoc nao!\n"; break; }
                cout << "Nhap ma lop: "; getline(cin, maLop);
                if (timChiSoLop(dsLop, soLop, maLop) == -1) { 
                    cout << "Ma lop khong ton tai!\n"; break; 
                }
                cout << "Nhap ngay hoc (dd/mm/yyyy): "; getline(cin, ngayHoc);
                timKiemDiemDanhTheoNgay(dsLop, soLop, maLop, ngayHoc);
                break;
            }

            case 7: {
                if (soLop == 0) { cout << "Khong co lop hoc nao!\n"; break; }
                cout << "Nhap ma lop: "; getline(cin, maLop);
                inDanhSachSinhVienVangNhieuNhat(dsLop, soLop, maLop);
                break;
            }

            case 0: {
                cout << "Thoat chuong trinh!\n";
                break;
            }

            default:
                cout << "Lua chon khong hop le, vui long nhap lai (0-7)!\n";
        } 
    }while (choice != 0) ; 
}
