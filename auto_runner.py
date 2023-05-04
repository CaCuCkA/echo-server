import socket
import threading


def send_message(sock, message):
    sock.send(message.encode())
    response = sock.recv(1024)
    print(response.decode())


def run_client(server_address):
    for i in range(100000):
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect(server_address)

        send_message(client_socket, "Hello, server!")

        client_socket.close()


if __name__ == '__main__':
    server_address = ('localhost', 12345)

    client_thread_1 = threading.Thread(target=run_client, args=(server_address,))
    client_thread_2 = threading.Thread(target=run_client, args=(server_address,))

    client_thread_1.start()
    client_thread_2.start()

    client_thread_1.join()
    client_thread_2.join()
