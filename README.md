# SFND 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

## Rubric Points

### MP.1 Data Buffer Optimization

I started out trying to design my own ring buffer using a custom class holding an
array, but after some research, implementing my own `.begin()` and `.end()` seemed
pretty difficult, so I ended up just writing a light wrapper around `std::deque`,
which seems to work quite well.

### MP.2 Keypoint Detection

HARRIS, FAST, BRISK, ORB, AKAZE, and SIFT all have standard APIs in OpenCV, so
adding them wasn't that hard. OpenCV docs leave a little to be desired at some points,
but overall this was pretty simple.

### MP.3 Keypoint Removal

The provided `cv::Rect` is convenient, as the `.contains()` methods allows for easy
bounds checking for each keypoint.

### MP.4 Keypoint Descriptors

Similar to MP.2, adding these in was not difficult due to the standard OpenCV API.

### MP.5 Descriptor Matching

FLANN was fairly easy to understand, but KNN took a bit of documentation searching
before I figured out how to set it up.

### MP.6 Descriptor Distance Ratio

This took me some time to figure out -- after some searching around I finally found
a snippet (and the associated paper!) that introduced the descriptor distance
ratio test, and I was able to adapt the algorithm for my usage without issue. It
wasn't immediately intuitive as to why this was beneficial, but after some thought
I came to appreciate the comparison between the two match distances!

### MP.7 Performance Evaluation 1

While a little brute force, this was a nice investigation into the nuances of each detector.
I considered setting up some sort of Command Line Interface and using a bash script
to run through each one, but I decided that would be a little overkill. Instead I
just switched the strings and reran each time.

### MP.8 Performance Evaluation 2

Increasingly brute force here -- with each combination of detector and descriptor,
the number of runs really adds up! Thankfully, leaving the descriptor distance ratio
test at 0.8 instead of switching it was well made the number of runs tractable.

### MP.9 Performance Evaluation 3

This was the most exciting -- logging the runtime gave me a great intuition for
how long each combination took, and looking at the number of keypoints generated
gave me an understanding of the time per keypoint for the descriptors.


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

| Detector | Descriptor | Num KeyPts (approx) | Detection (ms) | Extraction (ms) |
| -------- | ---------- | ------------------- | -------------- | --------------- |
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
