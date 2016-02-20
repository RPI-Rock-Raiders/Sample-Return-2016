#!/usr/bin/python

import numpy as np
import funct
from shape import shape

class icylinder(shape):

	def __init__(self, r, ax, ay, az, px, py, pz):
		shape.__init__(self)
		self.radius = r
		self.axisvect = np.array([ax,ay,az]).reshape(3,1)
		self.axispoint = np.array([px,py,pz]).reshape(3,1)

	def intersect(self, rays, ray0):
		nrays = rays.shape[0]
		
		dotvect = np.dot(rays,self.axisvect)
		Av = rays - np.tile(dotvect,[1,3])*np.tile(self.axisvect,[1,nrays]).T

		dp = ray0 - self.axispoint
		dotpv = np.dot(dp.T,self.axisvect)
		Cv = dp - dotpv*self.axisvect

		A = np.sum(Av*Av,axis=1).reshape(Av.shape[0],1)
		B = 2*np.dot(Av,Cv)
		C = np.dot(Cv.T,Cv)-self.radius*self.radius

		d = funct.qroots(A,B,C)
		d[np.isnan(d)] = -1
		return np.amin(d, axis=1).reshape(d.shape[0],1)
