#!/usr/bin/env python
# Brayden Hollis
# Point Cloud/Depth Image Generator

import rospy
from std_msgs.msg import String
import numpy as np
import math
from plane import iplane
from cylinder import icylinder

def sim_main():
	#parameters
	ypixels = 480.0
	xpixels = 640.0
	theta = 110.0
	phi = ypixels/xpixels*theta
	xmax = 15.0

	#camera
	center = np.array([(xpixels+1)/2, (ypixels+1)/2])
	xdif = np.dot(np.ones((ypixels,1), dtype=float),np.arange(xpixels).reshape(1,xpixels)+1.0)
	xdif -= center[0]
	xdif *= math.tan(theta/2*math.pi/180)/center[0]
	ydif = center[1] - np.dot(np.arange(ypixels).reshape(ypixels,1)+1.0,np.ones((1,xpixels), dtype=float))
	ydif *= math.tan(phi/2*math.pi/180)/center[1]
	rays = np.hstack([xdif.reshape(xdif.size,1), ydif.reshape(ydif.size,1), np.ones((xdif.size,1), dtype=float)])
	ray0 = np.array([0, 1, 0]).reshape(3,1)

	#ground plane
	groundplane = iplane(0,1,0,0,0,0)
	dg = groundplane.intersect(rays,ray0)
	print(dg.shape)

	#obstacle
	obs = icylinder(1,0,1,0,0,0,5)
	do = obs.intersect(rays,ray0)
	print(do.shape)
	k = np.arange(1)-1
	d = (k)**(.5)


def publish():
	sim_main()
	pub = rospy.Publisher('pointcloud_', String, queue_size=10)
	rospy.init_node('pointcloud_gen', anonymous=True)
	rate = rospy.Rate(10) # 10hz
	while not rospy.is_shutdown():
		hello_str = "hello world %s" % rospy.get_time()
		#rospy.loginfo(hello_str)
		pub.publish(hello_str)
		rate.sleep()

if __name__ == '__main__':
	try:
		publish()
	except rospy.ROSInterruptException:
		pass
