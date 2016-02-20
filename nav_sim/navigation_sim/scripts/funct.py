#!/usr/bin/python

import numpy as np

import warnings

def qroots(a,b,c):
	with warnings.catch_warnings():
		warnings.simplefilter("ignore")
		x1 = (-b + (b*b - 4*a*c)**(.5))*(2*a)**(-1)
		x2 = (-b - (b*b - 4*a*c)**(.5))*(2*a)**(-1)
	return np.hstack([x1,x2])
