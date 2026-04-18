import socket

def capitalize_buffer(buf: bytearray) -> None:
    for i in range(len(buf)):
        if 97 <= buf[i] <= 122:
            buf[i] -= 32

HOST = "127.0.0.1"
PORT = 9000

try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen()
        print(f"Listening on {HOST}:{PORT}")

        while True:
            conn, addr = s.accept()
            with conn:
                print(f"Connected by {addr}")
                while True:
                    data = conn.recv(4096)
                    if not data:
                        break

                    # --- RECEIVED ---
                    print(f"Received raw: {data}")
                    print(f"Received decoded: {data.decode(errors='replace')}")

                    buf = bytearray(data)
                    capitalize_buffer(buf)

                    # --- SENDING ---
                    print(f"Sending raw: {buf}")
                    print(f"Sending decoded: {buf.decode(errors='replace')}")

                    conn.sendall(buf)

except KeyboardInterrupt:
    print("\nServer stopped.")