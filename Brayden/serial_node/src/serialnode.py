#!/usr/bin/python

'''
serialnode.py - This node communicates with the PSOC and provides services
to send to the PSOC.

RPI Rock Raiders
6/4/15

Last Updated: Bryant Pong: 6/11/15 - 5:05 PM
'''

# Python Imports:
import rospy
import time
import serial 
from serial_node.srv import * 

# Serial objects:
ARDUINOPORT = "/dev/ttyACM0"
ARDUINOBAUD = 9600

MOTORPORT = "/dev/ttyUSB0"
MOTORBAUD = 9600

# FRONT LEFT WHEEL
FRTLFT = 128
FRTLFTFRD = 4
FRTLFTSCL = 127 #TUNE FOR EVEN SPEED

# FRONT RIGHT WHEEL
FRTRGT = 129
FRTRGTFRD = 4
FRTLFTSCL = 127 #TUNNE FOR EVEN SPEED

# BACK LEFT WHEEL
BCKLFT = 131
BCKLFTFRD = 4
BCKLFTSCL = 127 #TUNE FOR EVEN SPEED

# BACK RIGHT WHEEL
BCKRGT = 131
BCKRGTFRD = 0
BCKRGTSCL = 127 #TUNE FOR EVEN SPEED


motorserial = serial.Serial(MOTORPORT, MOTORBAUD, 8, 'N', 1)

'''
This is a helper function to send commands directly to the Sabertooth
motor drivers:

Data is in the format:
1) address
2) command
3) data

'''
def writeData(addr, cmd, data):
	chksum = (addr+cmd+data) & 127
	motorserial.write(chr(addr))
	motorserial.write(chr(cmd))
	motorserial.write(chr(data))
	motorserial.write(chr(chksum))

# cmd is an INTEGER 
def writeArduinoData(cmd):
	arduinoserial.write(chr(cmd))
	# Get the response back:
	return arduinoserial.readline()

'''
This service sends motor velocity commands to all four motors of the chassis.

This service expects the custom service "wheelvel.srv".
'''
def drive_service(req):
	# Get the target velocities of the motors:
	vels = [int(req.front_left*127), int(req.front_right*127), int(req.rear_right*127)]
	dirs = [4 if vel >=0 else 5 for vel in vels]
	motors = [128, 129, 130]

	# Send the velocities.  Back left motor is dead:
	for i in xrange(3):
		writeData(motors[i], dirs[i], abs(vels[i])) 

	return True


'''
Forward
'''
def forward_service(req):
	vels = [int(req.speed*FRTLFTSCL), int(req.speed*FRTRGTSCL), int(req.speed*BCKLFTSCL), int(req.speed*BCKRGTSCL)]
	dirs = [FRTLFTFRD FRTRGTFRD BCKLFTFRD BCKRGTFRD]
	motors = [FRTLFT, FRTRGT, BCKLFT, BCKRGT]

	for i in range(3):
		writeData(motors[i], dirs[i], abs(vels[i]))

	return True

'''
Backward
'''
def backward_service(req)
	vels = [int(req.speed*FRTLFTSCL), int(req.speed*FRTRGTSCL), int(req.speed*BCKLFTSCL), int(req.speed*BCKRGTSCL)]
	dirs = [FRTLFTFRD+1, FRTRGTFRD+1, BCKLFTFRD+1, BCKRGTFRD+1]
	motors = [FRTLFT, FRTRGT, BCKLFT, BCKRGT]

	for i in range(3):
		writeData(motors[i], dirs[i], abs(vels[i]))

	return True

'''
Turn Clockwise
'''
def turn_clockwise_service(req)
	vels = [int(req.speed*FRTLFTSCL), int(req.speed*FRTRGTSCL), int(req.speed*BCKLFTSCL), int(req.speed*BCKRGTSCL)]
	dirs = [FRTLFTFRD, FRTRGTFRD+1, BCKLFTFRD, BCKRGTFRD+1]
	motors = [FRTLFT, FRTRGT, BCKLFT, BCKRGT]

	for i in range(3):
		writeData(motors[i], dirs[i], abs(vels[i]))

	return True

'''
Turn CounterClockwise
'''
def turn_counterclockwise_service(req)
	vels = [int(req.speed*FRTLFTSCL), int(req.speed*FRTRGTSCL), int(req.speed*BCKLFTSCL), int(req.speed*BCKRGTSCL)]
	dirs = [FRTLFTFRD+1, FRTRGTFRD, BCKLFTFRD+1, BCKRGTFRD]
	motors = [FRTLFT, FRTRGT, BCKLFT, BCKRGT]

	for i in range(3):
		writeData(motors[i], dirs[i], abs(vels[i]))

	return True
	
'''
STOP
'''
def stop_service(req)
	vels = [int(0), int(0), int(0), int(0)]
	dirs = [FRTLFTFRD, FRTRGTFRD, BCKLFTFRD, BCKRGTFRD]
	motors = [FRTLFT, FRTRGT, BCKLFT, BCKRGT]

	for i in range(3):
		writeData(motors[i], dirs[i], abs(vels[i]))

	return True

def serial_server():
	rospy.init_node("serial_node_server")
	# Start all services:
	driveService = rospy.Service("wheel_vel", WheelVel, drive_service)
	forwardService = rospy.Service("fowardservice", drive_cmd, foward_service)
	backwardService = rospy.Service("backwardservice", drive_cmd, backward_service)
	cwService = rospy.Service("clockwiseservice", drive_cmd, turn_clockwise_service)
	ccwService = rospy.Service("counterclockwiseservice", drive_cmd, turn_counterclockwise_service)
	stopService = rospy.Service("stopservice", drive_cmd, stop_serive)
	
	rospy.spin()
	
if __name__ == "__main__":
	serial_server()

