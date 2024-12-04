import ctypes
import os
import requests
import re

# Đường dẫn tới file C đã biên dịch (thay đổi theo cấu trúc thư mục của bạn)
control_red = ctypes.CDLL('./move_to_red.so')
control_green = ctypes.CDLL('./move_to_green.so')
control_blue = ctypes.CDLL('./move_to_blue.so')
route = ctypes.CDLL('./route.so')

# URL ngrok của Colab
url = 'http://xxxxxx.ngrok.io/process'

# Hàm xử lý phản hồi từ Colab và gọi các hàm C
def process_response(output):
    if 'red' in output:
        print("Gọi hàm control_red...")
        control_red.control_red()
    elif 'green' in output:
        print("Gọi hàm control_green...")
        control_green.control_green()
    elif 'blue' in output:
        print("Gọi hàm control_blue...")
        control_blue.control_blue()
    elif 'rotate' in output:
        print("Gọi hàm route...")
        
        # Sử dụng biểu thức chính quy để trích xuất góc từ output
        match = re.search(r'rotates (right|left) (\d+)', output)
        
        if match:
            direction = match.group(1)  # 'right' hoặc 'left'
            angle = float(match.group(2))  # Trích xuất góc (ví dụ: 45)
            
            # Áp dụng quy tắc: nếu 'right', góc sẽ có dấu âm; nếu 'left', góc sẽ có dấu dương
            if direction == 'right':
                angle = -angle
            print(f"Góc đã xử lý: {angle} degrees")
            
            # Truyền góc vào hàm route với ctypes
            route.route(ctypes.c_int(angle))
        else:
            print("Không thể trích xuất góc từ output.")
    else:
        print("Không tìm thấy lệnh phù hợp.")

# Hàm gửi POST request tới Colab
def send_request(text):
    data = {"instruction": text}
    response = requests.post(url, json=data)
    if response.status_code == 200:
        response_data = response.json()
        print(f"Output: {response_data['output']}")
        return response_data['output']
    else:
        print("Lỗi: Không kết nối được với server.")
        return None

# Main loop
if __name__ == "__main__":
    while True:
        # Thay thế phần nhận dạng giọng nói bằng nhập liệu từ bàn phím
        text = input("Nhập lệnh điều khiển: ").strip().lower()
        if text:
            output = send_request(text)
            if output:
                process_response(output)
