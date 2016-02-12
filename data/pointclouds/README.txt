README - Point Clouds

This folder contains sample point clouds for testing navigation code
  (or more accurately it contains samples for designing code, tests will
  use this type of data but generated from code, in ROS)

Files
-------
README        - This file
flatfield.csv - sample point cloud of a flat field
pothole.csv   - sample point cloud of a flat field with a spherical pot-
              - hole centered 2 meters in front of 'camera'
obstacle.csv  - sample point cloud of a flat field with a cylindrical
              - obstacle centered 5 meters in front of 'camera'

Notes
-------
While the camera was still 1 meter off the ground, with a downward tilt of 
  10 degrees, the point cloud is adjusted so y = 0 is the default ground 
  plane.  In other words, the camera orientation and position only affect
  what is seen, but the coordinates are not affected.  Also these point
  clouds will always assume the 'camera is at the origin facing in the
  z direction.  The files have three column (x,y,z) that are comma separated
  with each row (or line) a new point.

Notes - bullet form
-------
 - camera: always above the origin
 - coordinate: x to the left, y is upward (towards sky), and z is forward
 - coordinate: (probably will change in future)
 - y = 0 is the general ground plane
 - future: add noise and holes (this will be more realistic)