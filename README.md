# HelibSorting

[NOTE : Documentation của thư viện là không chính xác,mong
các bạn chú ý.Các tác giả dường như không chú ý cập nhật
documentation]

Sử dụng thư viện Helib - Thư viện mã hóa đồng cấu sử dụng mô
hình mã hóa đồng cấu leveled BGV - để cài đặt thuật toán 
direct-sorting và Bionic-Sorting trên dữ liệu mã hóa.

Để chạy,trước tiên cài thư viện Helib.

Source code bao gồm 3 file : encrypter.cpp , decrypter.cpp
,cloud_sorter.cpp

encrypter dùng để biên dịch ra file mã hóa các số cần sắp 
xếp có ở trong file input_value.txt.Trả lại hai file
secF_encrypter.txt,file này chưa các thông tin mật dành 
cho decrypter.cpp giải mã sau khi nhận được kết quả để
trong file result.txt mà cloud_sorter sẽ trả lại,và file
sendF_encyrpter.txt file này sẽ dành để gửi cho cloud_sorter 
chứa các bản mã của đầu vào.

Biên dịch : 

g++ -g -O2 -o cloudsorter_x cloud_sorter.cpp  "đường dẫn fhe.a trong src của Helib" -L/usr/local/lib -lntl -lgmp -lm

g++ -g -O2 -o decrypter_x decrypter.cpp  "đường dẫn fhe.a trong src của Helib" -L/usr/local/lib -lntl -lgmp -lm

g++ -g -O2 -o encrypter_x encrypter.cpp  "đường dẫn fhe.a trong src của Helib" -L/usr/local/lib -lntl -lgmp -lm



