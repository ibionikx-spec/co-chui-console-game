Dưới đây là toàn bộ nội dung file **`README.md`** được thiết kế chuẩn hóa, trực quan và chuyên nghiệp bằng Markdown để bạn có thể copy trực tiếp vào dự án của mình:

```markdown
# 🎮 Console Game: Cờ Chùi (Nine Men's Morris) - C++ Version

Đồ án môn học xây dựng trò chơi **Cờ Chùi** (một biến thể của Nine Men's Morris) chạy trên nền tảng Console bằng ngôn ngữ C++. Dự án được tổ chức và phát triển theo mô hình kiến trúc **MVC (Model - View - Controller)** kết hợp với **AI Engine**, tối ưu hóa cho quy mô nhóm **6 thành viên**.

---

## 📌 1. Kiến Trúc Hệ Thống (Architecture)

Để tránh xung đột code (conflict) khi làm việc nhóm 6 người, dự án được chia làm 4 phân hệ độc lập:

* **Model:** Quản lý cấu trúc dữ liệu bàn cờ, trạng thái quân cờ và kiểm tra tính hợp lệ của luật chơi.
* **View:** Xử lý hiển thị đồ họa ASCII, xuất giao diện trực quan ra màn hình Console.
* **Controller:** Điều khiển luồng trò chơi (`Game Loop`), tiếp nhận input từ người dùng và điều phối các phân hệ khác.
* **AI Engine:** Module xử lý trí tuệ nhân tạo độc lập cho chế độ chơi với Máy (Bot).

### 💡 Giải thuật & Cấu trúc dữ liệu cốt lõi
* **Bàn cờ:** Biểu diễn bằng một mảng 1 chiều $24$ phần tử (`int board[24]`). Trong đó: `0` = Trống, `1` = Người chơi A, `2` = Người chơi B/Bot.
* **Danh sách kề:** Mảng hằng số lưu trữ các cặp vị trí liền kề để phục vụ cho việc tính toán di chuyển hợp lệ (Giai đoạn 2).
* **Danh sách Mill:** Mảng các bộ 3 vị trí tạo thành hàng thẳng để kiểm tra điều kiện ăn quân của đối thủ.

---

## 📂 2. Cấu Trúc Thư Mục Dự Án (Folder Structure)

```text
📁 CoChui_Console/
│
├── 📁 src/
│   ├── 📁 Model/
│   │   ├── Board.cpp         # Quản lý mảng bàn cờ, danh sách kề
│   │   └── GameRules.cpp # Logic kiểm tra nước đi, kiểm tra hàng 3 (Mill)
│   │
│   ├── 📁 View/
│   │   └── ConsoleUI.cpp # Vẽ giao diện ASCII, render màn hình
│   │
│   ├── 📁 Controller/
│   │   └── GameEngine.cpp # Vòng lặp chính (Game Loop), quản lý trạng thái
│   │
│   ├── 📁 AI/
│   │   ├── AI_EasyMed.cpp # Bot Rule-based (Ngẫu nhiên / Chặn/ Ăn ngay)
│   │   └── AI_Minimax.cpp # Bot Khó (Minimax + Alpha-Beta Pruning)
│   │
│   └── main.cpp                        # Điểm khởi chạy ứng dụng
│
├── 📁 data/                            # Thư mục lưu trữ file Save/Load game
├── 📁 docs/                            # Tài liệu báo cáo, Slide thuyết trình
└── README.md                           # Hướng dẫn này

```

---

## 👥 3. Phân Rã Công Việc & Phân Công Thành Viên (WBS)

| Thành viên | Vai trò | Nhiệm vụ chi tiết | File đảm nhận |
| --- | --- | --- | --- |
| **TV 1** | **Team Leader & Architect** | Thiết kế Cấu trúc dữ liệu tổng thể; Xây dựng `Game Loop` điều phối luồng chuyển phase; Tích hợp code tổng thể. | `main.cpp`, `GameEngine.*` |
| **TV 2** | **Game Logic Specialist** | Viết hàm kiểm tra nước đi hợp lệ (Phase 1 & 2); Logic kiểm tra hàng 3 (Mill); Xử lý ăn quân đối thủ. | `GameRules.*` |
| **TV 3** | **UI/UX Developer** | Thiết kế giao diện bàn cờ ASCII (3 hình vuông lồng nhau); Cập nhật màn hình; Xử lý nhập liệu (Tọa độ/Phím mũi tên). | `ConsoleUI.*` |
| **TV 4** | **AI Bot Developer (Easy/Med)** | Xây dựng Bot Rule-based: Chọn nước ngẫu nhiên (Dễ); Biết ưu tiên ăn quân hoặc chặn đối thủ tạo hàng 3 (Trung bình). | `AI_EasyMed.*` |
| **TV 5** | **AI Expert (Hard)** | Cài đặt thuật toán Minimax kết hợp Cắt tỉa Alpha-Beta; Thiết kế hàm đánh giá trạng thái bàn cờ (Evaluation Function). | `AI_Minimax.*` |
| **TV 6** | **Tester & Feature Developer** | Viết hàm kiểm tra điều kiện kết thúc game; Code tính năng Save/Load game và Undo/Redo; Làm báo cáo & slide. | `Board.*` (phần phụ), `docs/` |

---

## 🎮 4. Quy Trình Vận Hành Trò Chơi (Game Flow)

Trò chơi sẽ trải qua lần lượt các trạng thái (Phases) được quản lý chặt chẽ bởi `GameEngine`:

1. **Giai đoạn 1 (Đặt quân):** Mỗi người chơi lần lượt đặt 9 quân cờ của mình vào các vị trí trống trên bàn cờ.
2. **Giai đoạn 2 (Di chuyển quân):** Người chơi di chuyển một quân cờ sang vị trí trống liền kề có đường nối.
3. **Luật Ăn Quân (Mill):** Ở cả hai giai đoạn, nếu tạo thành một hàng 3 quân cùng màu thẳng hàng, người chơi được chỉ định xóa 1 quân bất kỳ của đối thủ (không nằm trong một hàng 3 khác trừ khi không còn lựa chọn nào khác).
4. **Kết thúc:** Game kết thúc khi một bên chỉ còn dưới 3 quân hoặc không còn nước đi hợp lệ.

---

## 🛠️ 5. Hướng Dẫn Biên Dịch & Chạy Game

### Yêu cầu hệ thống

* Trình biên dịch C++ hỗ trợ tiêu chuẩn **C++11** trở lên (GCC/G++, MSVC, Clang).
* Hệ điều hành: Windows (Sử dụng `<windows.h>`) hoặc Linux/macOS (Sử dụng `<ncurses.h>` nếu cần tối ưu giao diện).

### Biên dịch bằng Terminal (Command Line)

Di chuyển vào thư mục gốc của dự án và chạy lệnh sau:

```bash
# Biên dịch tất cả các file source code thành file thực thi CoChuiGame
g++ -std=c++11 src/main.cpp src/Model/*.cpp src/View/*.cpp src/Controller/*.cpp src/AI/*.cpp -o CoChuiGame

# Chạy game trên Windows
CoChuiGame.exe

# Chạy game trên Linux/macOS
./CoChuiGame

```

---

## 🤝 6. Quy Định Đóng Góp Code (Git Workflow)

Để đảm bảo tiến độ và không đè code lên nhau, các thành viên tuân thủ quy tắc:

1. Không được `push` trực tiếp lên nhánh `main`.
2. Mỗi thành viên tạo một nhánh riêng theo tên module: `feature/model`, `feature/view`, `feature/ai-hard`,...
3. Tạo **Pull Request (PR)** và cần được Team Leader (`TV 1`) duyệt sau khi đã qua kiểm tra lỗi từ Tester (`TV 6`) trước khi merge vào `main`.

```

```