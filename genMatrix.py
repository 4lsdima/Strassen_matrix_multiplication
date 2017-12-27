#!/usr/bin/python
#encoding: utf-8

import numpy as np
import sys



def is_number(s):
	try:
		float(s)
		return True
	except ValueError:
		return False

np.random.seed(19680801)

if __name__ == "__main__":
	r1=1000
	c1=1000
	maX=100
	f='f'
	if len(sys.argv)==5:
		if is_number(sys.argv[1]) and is_number(sys.argv[2]) and is_number(sys.argv[3]):
			r1=int(sys.argv[1])
			c1=int(sys.argv[2])
			maX=int(sys.argv[3])
			f=str(sys.argv[4])
	r2=c1
	c2=r1
	s=[]
	if f=='i':
		a=np.random.randint(-maX,maX,r1*c1)
		b=np.random.randint(-maX,maX,r2*c2)

	else:
		a=np.random.uniform(-maX,maX,r1*c1)
		b=np.random.uniform(-maX,maX,r2*c2)
    
	with open("A.dat", 'w') as f:
		f.write(str(r1)+"\n")
		f.write(str(c1)+"\n")
		for i in xrange(len(a)):
			f.write(str(a[i])+"\n")

	with open("B.dat", 'w') as f:
		f.write(str(r2)+"\n")
		f.write(str(c2)+"\n")
		for i in xrange(len(b)):
			f.write(str(b[i])+"\n")
