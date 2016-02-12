README - Depth Images

This folder contains sample depth images for testing navigation code
  (or more accurately it contains samples for designing code, tests will
  use this type of image but generated from code, in ROS)

Files
-------
README        - This file
flatfield.png - sample depth image of a flat field
pothole.png   - sample depth image of a flat field with a spherical pot-
              - hole centered 2 meters in front of 'camera'
obstacle.png  - sample depth image of a flat field with a cylindrical
              - obstacle centered 5 meters in front of 'camera'

Notes
-------
For the current depth images, the camera is 1 meter off the ground, with a
  downward tilt of 10 degrees.  Dark pixels mean far away and light pixels
  are close.  Pixels vary from 0 (black) to 1 (white), with 0 meaning >= 15
  meters forward and 1 meaning 0 meters forward.  Depth is measured from
  straight forward distance of the image plane, not the distance the points
  are from the focal point.  (||| vs \|/ distance, we use the first one for,
  depth and the second one for generating the image)

Notes - bullet form
-------
 - camera: 1 meter off ground
 - camera: 10 degrees tilted downwards
 - image: pixel values vary 0 (black) to 1 (white)
 - image: 0 pixel value -> >= 15 meters depth
 - image: 1 pixel value -> = 0 depth
 - image: computation depth = 15*(1-pixelvalue)