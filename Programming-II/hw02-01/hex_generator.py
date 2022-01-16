import sys

if __name__ == "__main__":
    str01 = sys.stdin.read()
    str02 = ""
    for i in str01:
        str02 += hex(ord(i))[2:]
    print(str02)