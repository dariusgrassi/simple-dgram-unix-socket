import socket
import time
import os

SOCKET_PATH = "/tmp/dgram_server_socket"
CLIENT_SOCKET_PATH = "/tmp/dgram_client_socket"
BUFFER_SIZE = 1024

def main():
    # Create a Unix domain socket
    client_socket = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
    
    try:
        # Remove the socket file if it already exists
        if os.path.exists(CLIENT_SOCKET_PATH):
            os.remove(CLIENT_SOCKET_PATH)
        
        # Bind the client socket to a file
        client_socket.bind(CLIENT_SOCKET_PATH)

        # Send 10 messages
        for i in range(10):
            message = f"Message {i+1} from Python client"
            print(f"Sending: {message}")
            
            # Send the message
            client_socket.sendto(message.encode(), SOCKET_PATH)
            
            # Wait for a response
            data, _ = client_socket.recvfrom(BUFFER_SIZE)
            print(f"Received: {data.decode()}")
            
            # Short delay between messages
            time.sleep(0.5)
    
    except socket.error as e:
        print(f"Socket error: {e}")
    
    finally:
        # Close the socket
        client_socket.close()
        # Remove the temporary socket file
        if os.path.exists(CLIENT_SOCKET_PATH):
            os.remove(CLIENT_SOCKET_PATH)
        print("Client socket closed")

if __name__ == "__main__":
    main()

