import socket
import threading

# Define the server IP address and port
SERVER_IP = "127.0.0.1"
SERVER_PORT = 12345

# Define the message to send to the server
MESSAGE = "Hello from the client!"


def send_echo():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    client_socket.connect((SERVER_IP, SERVER_PORT))
    client_socket.sendall(MESSAGE.encode())

    data = client_socket.recv(1024)

    # print("Received:", data.decode())

    client_socket.close()


NUM_CLIENTS = 10000


def main():
    for i in range(NUM_CLIENTS):
        t = threading.Thread(target=send_echo)
        t.start()


if __name__ == '__main__':
    main()
