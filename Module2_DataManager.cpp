
#include "Module2_DataManager.h"
#include "Module1_Models.h"
#include <iostream>
#include <cstdlib>
#include <cctype>

using namespace std;

// ==========================================
// CÁC HÀM TIỆN ÍCH (DÙNG NỘI BỘ TRONG FILE)
// ==========================================

bool moFileNhap(ifstream& fin, const string paths[], int len) {
    for (int i = 0; i < len; i++) {
        if (fin.is_open()) fin.close();
        fin.open(paths[i].c_str());
        if (fin.is_open()) return true;
    }
    return false;
}

static int chuyenThuStringSangSo(const string& thuToken) {
    string token = thuToken;
    for (char& c : token) {
        if (c == '_' || c == ' ') c = ' ';
        c = (char)toupper(static_cast<unsigned char>(c));
    }

    auto contains = [&](const string& substring) {
        return token.find(substring) != string::npos;
    };

    if (contains("HAI")) return 2;
    if (contains("BA")) return 3;
    if (contains("TU") || contains("T\xC6\xAF")) return 4;
    if (contains("NAM") || contains("N\xE1\xBA\xA1M")) return 5;
    if (contains("SAU") || contains("S\xE1\xBA\xA7U")) return 6;
    if (contains("BAY") || contains("B\xE1\xBA\xB7")) return 7;

// Nếu là chuỗi số hoặc dạng Thu_7 thì cố gắng lấy phần số
    string digits;
    for (char c : token) {
        if (isdigit(static_cast<unsigned char>(c))) {
            digits.push_back(c);
        } else if (!digits.empty()) {
            break;
        }
    }

    if (!digits.empty()) {
        try { return stoi(digits); } catch (...) { return 0; }
    }
    return 0;
}

static int trangThaiTextToCode(const string& text) {
    string s = text;
    for (char& c : s) {
        if (c == '_') c = ' ';
        c = (char)tolower(c);
    }
    if (s == "co mat" || s == "comat") return 1;
    if (s == "vang khong phep" || s == "vang_khong_phep" || s == "vangkhongphep") return 2;
    if (s == "co phep" || s == "co_phep" || s == "cophep") return 3;
    return 0;
}

void tachChuoi(const string& s, char delim, string* parts, int& count, int maxParts) {
    count = 0;
    string temp = "";
    for (size_t i = 0; i < s.length() && count < maxParts; i++) {
        if (s[i] == delim) {
            if (!temp.empty()) {
                parts[count++] = temp;
                temp = "";
            }
        } else {
            temp += s[i];
        }
    }
    if (!temp.empty() && count < maxParts) {
        parts[count++] = temp;
    }
}

// ==========================================
// CÁC HÀM THAO TÁC DỮ LIỆU CƠ BẢN
// ==========================================

int timChiSoLop(LopHoc** dsLop, int soLop, string maLop) {
    for (int i = 0; i < soLop; i++) {
        if (dsLop[i]->maLop == maLop) return i;
    }
    return -1;
}

LopHoc* chonLop(LopHoc** dsLop, int soLop) {
    if (soLop == 0) {
        cout << "Khong co lop hoc nao trong he thong!\n";
        return NULL;
    }
    cout << "\n===== DANH SACH LOP HOC =====\n";
    for (int i = 0; i < soLop; i++) {
        cout << i + 1 << ". " << dsLop[i]->maLop << " - " << dsLop[i]->tenLop
             << " (Si so: " << dsLop[i]->siSo << ")\n";
    }
    cout << "==============================\n";
    int choice;
    cout << "Chon lop (1-" << soLop << "): ";
    cin >> choice; cin.ignore();
    if (choice >= 1 && choice <= soLop) return dsLop[choice - 1];
    cout << "Lua chon khong hop le!\n";
    return NULL;
}


static void ensureDataFolder() {
    #ifdef _WIN32
        system("if not exist data mkdir data");
    #else
        system("mkdir -p data");
    #endif
}

static string buildDataPath(const string& tenFile) {
    if (tenFile.rfind("data/", 0) == 0 || tenFile.rfind("data\\", 0) == 0) {
        return tenFile;
    }
    return string("data/") + tenFile;
}

static string attendanceCodeToText(int code) {
    switch (code) {
        case 1: return "Co_mat";
        case 2: return "Vang_khong_phep";
        case 3: return "Vang_co_phep";
        default: return "Chua_diem_danh";
    }
}

void luuDuLieuLopHoc(LopHoc** dsLop, int soLop, string tenFile) {
    ensureDataFolder();
    string fullPath = buildDataPath(tenFile);
    ofstream fout(fullPath.c_str());
    if (!fout.is_open()) { cout << "Khong the mo file de luu: " << fullPath << "\n"; return; }

    fout << "# FILE DU LIEU LOP HOC" << endl;
    fout << "# Format: LOPHOC|MaLop|TenLop" << endl;
    fout << "#         TIETHOC|Thu|Tiet|Phong|MonHoc" << endl;
    fout << "#         DANHSACH|MaSV1,MaSV2,..." << endl;
    fout << "#--------------------------------------------------" << endl;

    for (int k = 0; k < soLop; k++) {
        LopHoc& lop = *dsLop[k];
        fout << "LOPHOC|" << lop.maLop << "|" << lop.tenLop << "\n";
        for (int i = 0; i < lop.soTiet; i++) {
            fout << "TIETHOC|" << lop.dsTiet[i].thu << "|" << lop.dsTiet[i].tiet << "|" << lop.dsTiet[i].phong << "|" << lop.dsTiet[i].monHoc << "\n";
        }
        fout << "DANHSACH|";
        NodeSinhVien* curr = lop.head;
        bool first = true;
        while (curr) {
            if (!first) fout << ",";
            fout << curr->data.MSSV;
            first = false;
            curr = curr->next;
        }
        fout << "\n";
    }
    fout.close();
    cout << "Da luu du lieu lop hoc vao file " << fullPath << endl;
}

void luuDuLieuSinhVien(LopHoc** dsLop, int soLop, string tenFile) {
    ensureDataFolder();
    string fullPath = buildDataPath(tenFile);
    ofstream fout(fullPath.c_str());
    if (!fout.is_open()) { cout << "Khong the mo file de luu: " << fullPath << "\n"; return; }

    fout << "# FILE DU LIEU SINH VIEN" << endl;
    fout << "# Format: MaSV|HoTen|NgaySinh" << endl;
    fout << "#--------------------------------------------------" << endl;

    for (int k = 0; k < soLop; k++) {
        NodeSinhVien* curr = dsLop[k]->head;
        while (curr) {
            fout << curr->data.MSSV << "|" << curr->data.hoTen << "|" << curr->data.ngaySinh << "\n";
            curr = curr->next;
        }
    }
    fout.close();
    cout << "Da luu du lieu sinh vien vao file " << fullPath << endl;
}

void luuDuLieuDiemDanh(LopHoc** dsLop, int soLop, string tenFile) {
    ensureDataFolder();
    string fullPath = buildDataPath(tenFile);
    ofstream fout(fullPath.c_str());
    if (!fout.is_open()) { cout << "Khong the mo file de luu: " << fullPath << "\n"; return; }

    fout << "# FILE DU LIEU DIEM DANH" << endl;
    fout << "# Format: MaSV|MaLop|NgayDiemDanh|TrangThai" << endl;
    fout << "# TrangThai: Co_mat | Vang_co_phep | Vang_khong_phep" << endl;
    fout << "#--------------------------------------------------" << endl;

    for (int k = 0; k < soLop; k++) {
        LopHoc* lop = dsLop[k];
        NodeSinhVien* curr = lop->head;
        while (curr) {
            for (int i = 0; i < curr->data.tongSoBuoi; i++) {
                fout << curr->data.MSSV << "|" << lop->maLop << "|" << curr->data.dsDiemDanh[i].date << "|" << attendanceCodeToText(curr->data.dsDiemDanh[i].trangThai) << "\n";
            }
            curr = curr->next;
        }
    }
    fout.close();
    cout << "Da luu du lieu diem danh vao file " << fullPath << endl;
}

void luuDuLieu(LopHoc** dsLop, int soLop, string tenFile) {
    if (tenFile.find("lophoc") != string::npos) {
        luuDuLieuLopHoc(dsLop, soLop, tenFile);
    } else if (tenFile.find("sinhvien") != string::npos) {
        luuDuLieuSinhVien(dsLop, soLop, tenFile);
    } else if (tenFile.find("attendance") != string::npos) {
        luuDuLieuDiemDanh(dsLop, soLop, tenFile);
    } else {
        cout << "Khong xac dinh loai file de luu: " << tenFile << "\n";
    }
}


void docDuLieuTu3FileTXT(LopHoc**& dsLop, int& soLop, int& capacityLop) {
    // 1. Tự cài đặt Mảng động lưu trữ Thông tin Sinh viên (Thay thế cho map<string, string>)
    struct ThongTinSV { string mssv; string hoTen; string ngaySinh; };
    int capSV = 10;
    int soLuongSV = 0;
    ThongTinSV* arrSV = new ThongTinSV[capSV];

    ifstream finSV;
    const string svPaths[] = { "data/sinhvien.txt" };
    if (!moFileNhap(finSV, svPaths, 1)) {
        cout << "Khong the mo file sinhvien.txt. Kiem tra thu muc data!" << endl;
        delete[] arrSV; return;
    }
    string line;
    while (getline(finSV, line)) {
        if (line.empty() || line[0] == '#') continue;
        string p[3]; int c; tachChuoi(line, '|', p, c, 3);
        if (c >= 3) {
            // Mở rộng mảng tự tạo nếu đầy
            if (soLuongSV >= capSV) {
                capSV *= 2;
                ThongTinSV* t = new ThongTinSV[capSV];
                for (int i = 0; i < soLuongSV; i++) t[i] = arrSV[i];
                delete[] arrSV; arrSV = t;
            }
            arrSV[soLuongSV].mssv = p[0];
            arrSV[soLuongSV].hoTen = p[1];
            arrSV[soLuongSV].ngaySinh = p[2];
            soLuongSV++;
        }
    }
    finSV.close();

    // 2. Đọc file lớp học
    ifstream finLop;
    const string lopPaths[] = { "data/lophoc.txt" };
    if (!moFileNhap(finLop, lopPaths, 1)) {
        cout << "Khong the mo file lophoc.txt. Kiem tra thu muc data!" << endl;
        delete[] arrSV; return;
    }

    soLop = 0;
    LopHoc* currentLop = nullptr;

    while (getline(finLop, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line.find("LOPHOC|") == 0) {
            string p[3]; int c; tachChuoi(line, '|', p, c, 3);
            if (c < 3) continue;

            if (soLop >= capacityLop) {
                capacityLop *= 2;
                LopHoc** newArr = new LopHoc*[capacityLop];
                for (int i = 0; i < soLop; i++) newArr[i] = dsLop[i];
                delete[] dsLop; dsLop = newArr;
            }
            currentLop = new LopHoc();
            currentLop->maLop = p[1];
            currentLop->tenLop = p[2];
            dsLop[soLop++] = currentLop;
        }
        else if (line.find("TIETHOC|") == 0 && currentLop) {
            string p[10]; int c; tachChuoi(line, '|', p, c, 10);
            if (c >= 4) {
                int thu = chuyenThuStringSangSo(p[1]);
                string monHoc = (c >= 5) ? p[4] : "";
                themTietHoc(*currentLop, thu, p[2], p[3], monHoc);
            }
        }
        else if (line.find("DANHSACH|") == 0 && currentLop) {
            string svList = line.substr(9);
            
            // Tự cài đặt Mảng động lưu trữ MSSV (Thay thế cho vector<string>)
            int capMSSV = 9;
            int soMSSV = 0;
            string* mssvArr = new string[capMSSV];
            
            string temp = "";
            for (size_t i = 0; i <= svList.length(); i++) {
                if (i == svList.length() || svList[i] == ',') {
                    if (!temp.empty()) {
                        if (soMSSV >= capMSSV) {
                            capMSSV *= 2;
                            string* t = new string[capMSSV];
                            for (int j = 0; j < soMSSV; j++) t[j] = mssvArr[j];
                            delete[] mssvArr; mssvArr = t;
                        }
                        mssvArr[soMSSV++] = temp;
                        temp.clear();
                    }
                } else if (svList[i] != ' ' && svList[i] != '\r' && svList[i] != '\n') {
                    temp += svList[i];
                }
            }

            int stt = 1;
            for (int k = 0; k < soMSSV; k++) {
                string mssvCanTim = mssvArr[k];
                // Linear Search trong mảng arrSV tự tạo thay cho map.count()
                for (int i = 0; i < soLuongSV; i++) {
                    if (arrSV[i].mssv == mssvCanTim) {
                        SinhVien s(stt++, mssvCanTim, arrSV[i].hoTen, arrSV[i].ngaySinh);
                        themSinhVienVaoLop(*currentLop, s);
                        break; 
                    }
                }
            }
            delete[] mssvArr; // Dọn dẹp RAM
        }
    }
    finLop.close();
    delete[] arrSV; // Dọn dẹp RAM
    // 3. Đọc điểm danh
    ifstream finAtt;
    const string attPaths[] = { "data/attendance_records.txt" };
    if (!moFileNhap(finAtt, attPaths, 1)) {
        cout << "Khong the mo file attendance_records.txt. Kiem tra thu muc data!" << endl;
        return;
    }
    while (getline(finAtt, line)) {
        if (line.empty() || line[0] == '#') continue;
        string p[4]; int c; tachChuoi(line, '|', p, c, 4);
        if (c >= 4) {
            int idx = timChiSoLop(dsLop, soLop, p[1]);
            if (idx != -1) {
                LopHoc* lop = dsLop[idx];
                NodeSinhVien* curr = lop->head;
                int code = trangThaiTextToCode(p[3]);
                while (curr != nullptr) {
                    if (curr->data.MSSV == p[0]) {
                        themNgayDiemDanh(curr->data, p[2], code);
                        break;
                    }
                    curr = curr->next;
                }
            }
        }
    }
    finAtt.close();
}

