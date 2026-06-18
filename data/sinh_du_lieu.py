import os
import random
from datetime import datetime, timedelta


# --- CẤU HÌNH DỮ LIỆU MẪU ĐỂ RANDOM ---
HO_VN = ["Nguyễn", "Trần", "Lê", "Phạm", "Hoàng", "Huỳnh", "Phan", "Vũ", "Võ", "Đặng", "Bùi", "Đỗ", "Hồ", "Ngô", "Dương", "Lý", "Đoàn", "Trịnh", "Đinh"]
DEM_VN = ["Văn", "Thị", "Hữu", "Ngọc", "Minh", "Đức", "Hoàng", "Xuân", "Thanh", "Gia", "Hải", "Tuấn", "Mai", "Thu", "Trọng", "Bảo", "Phương", "Kim", "Quang"]
TEN_VN = ["Anh", "Bảo", "Chi", "Dũng", "Đạt", "Giang", "Hà", "Hải", "Huy", "Khang", "Linh", "Long", "Nam", "Nga", "Phát", "Phương", "Quân", "Quỳnh", "Thảo", "Trang", "Trí", "Uyên", "Vinh", "Yến", "Vân", "Sơn", "Oanh", "Thắng", "Hiếu", "Bình", "Phúc", "Đào", "Dung", "Em", "Khoa", "Tân"]


TEN_MON_HOC = ["Giải tích 1", "Giải tích 2", "Giải tích 3", "Đại số", "Kĩ thuật lập trình", "Cơ sở dữ liệu", "Cấu trúc dữ liệu và giải thuật", "Xác suất thống kê", "Mạng máy tính", "Hệ điều hành", "Tiết hoc Mac - LêNin"]
KHOA_NHOM = ["K69 Nhom 1", "K69 Nhom 2", "K70 Nhom 1", "K70 Nhom 3", "K70 Nhom 2"]


# Định nghĩa các thứ đi kèm với mã của thư viện datetime (Monday=0, Tuesday=1,..., Sunday=6)
THU_MAP = {
    "Thu_Hai": 2,
    "Thu_Ba": 3,
    "Thu_Tư": 4,
    "Thu_Năm": 5,
    "Thu_Sáu": 6,
    "Thu_Bảy": 7
}  
TIET = ["Tiet_1-3", "Tiet_4-6", "Tiet_7-9", "Tiet_10-12"]
PHONG_HOC = ["D3-101", "D9-102", "D3,5-103", "D5-201", "D7-202", "D6-301", "D8-302", "D3-201", "D5-101", "D5-102","D3,5-201"]


TRANG_THAI_DD = ["Co_mat", "Vang_co_phep", "Vang_khong_phep"]


# --- HÀM HỖ TRỢ SẮP XẾP TÊN TIẾNG VIỆT ---
VN_ALPHABET = "aàảãáạăằẳẵắặâầẩẫấậbcdđeèẻẽéẹêềểễếệghiìỉĩíịklmnoòỏõóọôồổỗốộơờởỡớợpqrstuùủũúụưừửữứựvxyỳỷỹýỵ"
VN_CHAR_MAP = {c: i for i, c in enumerate(VN_ALPHABET)}


def vn_sort_key(full_name):
    parts = full_name.strip().lower().split()
    if not parts: return ()
    ten = parts[-1]
    ho = parts[0]
    dem = " ".join(parts[1:-1]) if len(parts) > 2 else ""
   
    def word_to_key(word):
        return tuple(VN_CHAR_MAP.get(c, 999) for c in word)
    return (word_to_key(ten), word_to_key(dem), word_to_key(ho))


# --- HÀM TÌM CÁC NGÀY HỌC TRONG 6 THÁNG QUA ---
def get_class_dates_last_6_months(target_weekdays):
    """Trả về danh sách các ngày (dd/mm/yyyy) trong 6 tháng qua trùng với các thứ trong target_weekdays"""
    end_date = datetime.now() # Hiện tại là tháng 06/2026
    start_date = end_date - timedelta(days=6*30) # Lùi lại 180 ngày (6 tháng)
   
    dates = []
    current_date = start_date
    while current_date <= end_date:
        if current_date.weekday() in target_weekdays:
            dates.append(current_date.strftime("%d/%m/%Y"))
        current_date += timedelta(days=1)
    return dates


# --- CHƯƠNG TRÌNH CHÍNH ---
def generate_mock_data(num_students=1000, num_classes=20):
   
    # 1. SINH DỮ LIỆU SINH VIÊN
    student_ids = random.sample(range(202200001, 202599999 + 1), num_students)
    students_list = []
    students_dict = {}
   
    for sid in student_ids:
        ho_ten = f"{random.choice(HO_VN)} {random.choice(DEM_VN)} {random.choice(TEN_VN)}"
        ngay_sinh = (datetime(2003, 1, 1) + timedelta(days=random.randint(0, 4* 365))).strftime("%d/%m/%Y")
       
        sv = {"ma_sv": str(sid), "ho_ten": ho_ten, "ngay_sinh": ngay_sinh}
        students_list.append(sv)
        students_dict[str(sid)] = sv


    # 2. SINH DỮ LIỆU LỚP HỌC & ĐIỂM DANH
    classes_list = []
    attendance_records = []
   
    class_ids = random.sample(range(100000, 999999), num_classes)
   
    for cid in class_ids:
        cid_str = str(cid)
        ten_lop = f"{random.choice(TEN_MON_HOC)} - {random.choice(KHOA_NHOM)}"
       
        # Sinh ngẫu nhiên từ 1 đến 2 buổi học/tuần cho lớp
        chosen_thurs = random.sample(list(THU_MAP.keys()), random.randint(1, 2))
       
        tiet_hoc_list = []
        target_weekdays = []
        for thu in chosen_thurs:
            tiet_hoc_list.append(f"{thu}|{random.choice(TIET)}|{random.choice(PHONG_HOC)}")
            target_weekdays.append(THU_MAP[thu])
           
        # Chọn đúng 50 sinh viên cho lớp này & SẮP XẾP theo ABC tiếng Việt
        class_students_ids = random.sample(list(students_dict.keys()), 50)
        class_students = [students_dict[sid] for sid in class_students_ids]
        class_students.sort(key=lambda sv: vn_sort_key(sv["ho_ten"]))
        sorted_student_ids = [sv["ma_sv"] for sv in class_students]
       
        classes_list.append({
            "ma_lop": cid_str,
            "ten_lop": ten_lop,
            "tiet_hoc": tiet_hoc_list,
            "danh_sach_sv": sorted_student_ids
        })
       
        # Lấy lịch các ngày học thực tế rơi vào đúng thứ đó trong 6 tháng qua
        class_dates = get_class_dates_last_6_months(target_weekdays)
       
        # Ghi nhận điểm danh xếp dọc theo từng sinh viên
        for sid in sorted_student_ids:
            for date_str in class_dates:
                # Tỉ lệ đi học: 88% Co_mat, 5% Vang_co_phep, 7% Vang_khong_phep
                status = random.choices(TRANG_THAI_DD, weights=[88, 5, 7])[0]
                attendance_records.append({
                    "ma_sv": sid,
                    "ma_lop": cid_str,
                    "ngay_diem_danh": date_str,
                    "trang_thai": status
                })


    # --- GHI FILE SINHVIEN.TXT ---
    with open("data/sinhvien.txt", "w", encoding="utf-8") as f:
        f.write("# FILE DU LIEU SINH VIEN\n")
        f.write("# Format: MaSV|HoTen|NgaySinh\n")
        f.write("#---------------------------------------------------------\n")
        for sv in students_list:
            f.write(f"{sv['ma_sv']}|{sv['ho_ten']}|{sv['ngay_sinh']}\n")
           
    # --- GHI FILE LOPHOC.TXT ---
    with open("data/lophoc.txt", "w", encoding="utf-8") as f:
        f.write("# FILE DU LIEU LOP HOC\n")
        f.write("# Format:\n")
        f.write("# LOPHOC|MaLop|TenLop\n")
        f.write("# TIETHOC|ThuThu|TietSo|PhongHoc\n")
        f.write("# DANHSACH|MaSV1,MaSV2,...\n")
        f.write("#---------------------------------------------------------\n")
       
        for idx, lh in enumerate(classes_list):
            f.write(f"LOPHOC|{lh['ma_lop']}|{lh['ten_lop']}\n")
            for th in lh['tiet_hoc']:
                f.write(f"TIETHOC|{th}\n")
            f.write(f"DANHSACH|{','.join(lh['danh_sach_sv'])}\n")
            f.write("#---------------------------------------------------------\n")
            if idx < len(classes_list) - 1:
                f.write("\n")
           
    # --- GHI FILE ATTENDANCE_RECORDS.TXT ---
    with open("data/attendance_records.txt", "w", encoding="utf-8") as f:
        f.write("# FILE DU LIEU DIEM DANH\n")
        f.write("# Format: MaSV|MaLop|NgayDiemDanh|TrangThai\n")
        f.write("# TrangThai: Co_mat | Vang_co_phep | Vang_khong_phep\n")
        f.write("#---------------------------------------------------------\n")
        for ar in attendance_records:
            f.write(f"{ar['ma_sv']}|{ar['ma_lop']}|{ar['ngay_diem_danh']}|{ar['trang_thai']}\n")


    print("Sinh dữ liệu thành công! Đã cập nhật lịch điểm danh 6 tháng gần nhất dựa trên thời khóa biểu.")


if __name__ == "__main__":
    generate_mock_data(num_students=1000, num_classes=20)

