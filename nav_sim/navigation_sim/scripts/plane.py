#!/usr/bin/python

import numpy as np

class iplane:
	def __init__(self,nx,ny,nz,px,py,pz):
		self.normal = np.array([nx, ny, nz]).reshape(3,1)
		self.point = np.array([px, py, pz]).reshape(3,1)

	def intersect(self,rays,ray0):
        	d = np.dot(rays,self.normal)**(-1)
        	d_ = np.dot((self.point-ray0).reshape(1,3),self.normal)
        	return d_[0]*d

