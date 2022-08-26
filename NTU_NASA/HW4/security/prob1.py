import os
import string

def decrypt(text, key):
	cyp = [ ord(c) - ord('a') for c in text]
	res = []


	for i in range(len(cyp) - 8, -1, -8):
		l, r = (i, i + 8)
		if(l != (len(cyp) - 8)):
			key = (key - cyp[r - 1]) % 26
		res = [ (c - key) % 26 if 0 <= c < 26 else c for c in cyp[l:r] ] + res
	
	res = ''.join( chr(c + ord('a')) for c in res)
	return res
		


if __name__ == '__main__':
	text = input()
	for i in range(0, 26):
		print(decrypt(text, i))
