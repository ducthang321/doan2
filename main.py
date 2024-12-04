import ctypes
import os
import speech_recognition as sr
import requests

# Đường dẫn tới file C đã biên dịch (thay đổi theo cấu trúc thư mục của bạn)
control_red = ctypes.CDLL('./move_to_red.so')
control_green = ctypes.CDLL('./move_to_green.so')
control_blue = ctypes.CDLL('./move_to_blue.so')
route = ctypes.CDLL('./route.so')

# URL ngrok của Colab
url = 'http://xxxxxx.ngrok.io/process'

# Hàm nhận dạng giọng nói
def speech_to_text():
    recognizer = sr.Recognizer()
    with sr.Microphone() as source:
        print("Nói điều khiển:")
        try:
            audio = recognizer.listen(source)
            text = recognizer.recognize_google(audio, language="en-US")
            print(f"Bạn đã nói: {text}")
            return text.lower()
        except sr.UnknownValueError:
            print("Không nhận dạng được âm thanh.")
            return None
        except sr.RequestError as e:
            print(f"Lỗi từ Google Speech Recognition: {e}")
            return None

# Hàm xử lý phản hồi từ Colab và gọi các hàm C
import re


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
        print("Lỗi: 404notfound.")
        return None

# Main loop
if __name__ == "__main__":
    while True:
        text = speech_to_text()
        if text:
            output = send_request(text)
            if output:
                process_response(output)
