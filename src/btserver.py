#!/usr/bin/env python
import bluetooth


def startServer():
    server_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)

    port = 1
    server_sock.bind(("", port))
    server_sock.listen(1)

    sock,addr = server_sock.accept()
    print("Connection established with", addr)

    sock.send("Some test string")

    server_sock.close()
    sock.close()

if __name__=='__main__':
    startServer()
