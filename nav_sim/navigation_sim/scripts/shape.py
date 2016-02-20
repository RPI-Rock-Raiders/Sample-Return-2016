#!/usr/bin/python

import numpy as np

class shape:

	def __init__(self):
		self = self

	def intersect(self,rays,ray0):
		return np.zeros((rays.size[0],1), dtype=float)
