# SFND 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

The idea of the camera course is to build a collision detection system - that's the overall goal for the Final Project. As a preparation for this, you will now build the feature tracking part and test various detector / descriptor combinations to see which ones perform best. This mid-term project consists of four parts:

* First, you will focus on loading images, setting up data structures and putting everything into a ring buffer to optimize memory load.
* Then, you will integrate several keypoint detectors such as HARRIS, FAST, BRISK and SIFT and compare them with regard to number of keypoints and speed.
* In the next part, you will then focus on descriptor extraction and matching using brute force and also the FLANN approach we discussed in the previous lesson.
* In the last part, once the code framework is complete, you will test the various algorithms in different combinations and compare them with regard to some performance measures.

See the classroom instruction and code comments for more details on each of these parts. Once you are finished with this project, the keypoint matching part will be set up and you can proceed to the next lesson, where the focus is on integrating Lidar points and on object detection using deep-learning.

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors.
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
  ```
  # first, clone opencv and opencv_contrib
  cd opencv
  mkdir build
  cd build
  cmake -D OPENCV_ENABLE_NONFREE=ON -D OPENCV_EXTRA_MODULES_PATH=/Users/daniel/code/opencv_contrib/modules ..
  make -j
  ```
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
1. Make a build directory in the top level directory: `mkdir build && cd build`
1. Set path: `export CMAKE_PREFIX_PATH="/Users/daniel/code/opencv/build`
1. Compile: `cmake .. && make`
1. Run it: `./2D_feature_tracking`.

## Runtime evaluation

| Detector | Descriptor | Num KeyPts | Detection (ms) | Extraction (ms) |
| -------- | ---------- | ---------- | -------------- | --------------- |
| SHITOMASI | BRISK | 1300  | 7 | 1 |
| SHITOMASI | ORB | 1300  | 7 | 0.5 |
| SHITOMASI | FREAK | 1300  | 7 | 0.35 |
| SHITOMASI | AKAZE | 1300  | 7 | 0.35 |
| SHITOMASI | SIFT  | 1300  | 7 | 0.45 |
| HARRIS  | BRISK | 10  | 7 | 0.21 |
| HARRIS  | ORB | 10  | 7 | 0.16 |
| HARRIS  | FREAK | 10  | 7 | 0.2 |
| HARRIS  | AKAZE | 10  | 7 | 0.16 |
| HARRIS  | SIFT  | 10  | 8 | 0.22 |
| FAST  | BRISK | 5000  | 1.5 | 3.1 |
| FAST  | ORB | 5000  | 1.5 | 0.77 |
| FAST  | FREAK | 5000  | 1.5 | 0.95 |
| FAST  | AKAZE | 5000  | 1.5 | 0.75 |
| FAST  | SIFT  | 5000  | 1.5 | 0.85 |
| BRISK | BRISK | 2500  | 150 | 2.3 |
| BRISK | ORB | 2500  | 150 | 0.55 |
| BRISK | FREAK | 2500  | 150 | 0.55 |
| BRISK | AKAZE | 2500  | 150 | 0.61 |
| BRISK | SIFT  | 2500  | 150 | 0.56 |
| ORB | BRISK | 500 | 4.5 | 1.1 |
| ORB | ORB | 500 | 4.5 | 0.45 |
| ORB | FREAK | 500 | 4.5 | 0.34 |
| ORB | AKAZE | 500 | 4.5 | 0.41 |
| ORB | SIFT  | 500 | 4.5 | 0.55 |
| AKAZE | BRISK | 1300  | 45  | 1.4 |
| AKAZE | ORB | 1300  | 45  | 1.3 |
| AKAZE | FREAK | 1300  | 45  | 1.2 |
| AKAZE | AKAZE | 1300  | 45  | 0.8 |
| AKAZE | SIFT  | 1300  | 45  | 1.2 |
| SIFT  | BRISK | 1400  | 72  | 1.1 |
| SIFT  | ORB | 1400  | 72  | 1.2 |
| SIFT  | FREAK | 1400  | 72  | 1.1 |
| SIFT  | AKAZE | 1400  | 72  | 1.1 |
| SIFT  | SIFT | 1400  | 72  | 1.1 |

## Analysis

For a real time autonomous system, I recommend the following detector / descriptor combinations:

```
FAST / ORB
FAST / AKAZE
FAST / SIFT
```

FAST produces a significant amount of keypts, and does so very quickly. The accuracy remains to be seen, but the runtime is very attractive.
