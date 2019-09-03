#include <numeric>
#include <stdexcept>
#include "matching2D.hpp"

using namespace std;

// Find best matches for keypoints in two camera images based on several matching methods
void matchDescriptors(std::vector<cv::KeyPoint> &kPtsSource, std::vector<cv::KeyPoint> &kPtsRef, cv::Mat &descSource, cv::Mat &descRef,
                      std::vector<cv::DMatch> &matches, std::string matcherType, std::string selectorType) {
    bool crossCheck = false;
    cv::Ptr<cv::DescriptorMatcher> matcher;

    if (matcherType.compare("MAT_BF") == 0) {
        int normType = cv::NORM_HAMMING;
        matcher = cv::BFMatcher::create(normType, crossCheck);
    } else if (matcherType.compare("MAT_FLANN") == 0) {
        // take care of the datatype you are inputting to FLANN!
        matcher = cv::FlannBasedMatcher::create();
    }

    // perform matching task
    if (selectorType.compare("SEL_NN") == 0) { // nearest neighbor (best match)
        matcher->match(descSource, descRef, matches); // Finds the best match for each descriptor in desc1
    } else if (selectorType.compare("SEL_KNN") == 0) { // k nearest neighbors (k=2)
        int k = 2;
        std::vector<std::vector<cv::DMatch>> kmatches;
        matcher->knnMatch(descSource, descRef, kmatches, k);

        for (auto _kmatch : kmatches) {
            if (_kmatch.size() >= 2) {
                cv::DMatch match1 = _kmatch[0];
                cv::DMatch match2 = _kmatch[1];
                // descriptor distance ratio filtering with t=0.8
                if (match1.distance < 0.8*match2.distance) {
                    matches.push_back(match1);
                }
            }
        }
    }
}

// Use one of several types of state-of-art descriptors to uniquely identify keypoints
void descKeypoints(vector<cv::KeyPoint> &keypoints, cv::Mat &img, cv::Mat &descriptors, string descriptorType) {
    // select appropriate descriptor
    cv::Ptr<cv::DescriptorExtractor> extractor;
    if (descriptorType.compare("BRISK") == 0) {
        int threshold = 30;        // FAST/AGAST detection threshold score.
        int octaves = 3;           // detection octaves (use 0 to do single scale)
        float patternScale = 1.0f; // apply this scale to the pattern used for sampling the neighbourhood of a keypoint.

        extractor = cv::BRISK::create(threshold, octaves, patternScale);

    // BRIEF, ORB, FREAK, AKAZE and SIFT.
    } else if (descriptorType.compare("BREIF")) {
        extractor = cv::xfeatures2d::BriefDescriptorExtractor::create();
    } else if (descriptorType.compare("ORB")) {
        extractor = cv::ORB::create();
    } else if (descriptorType.compare("FREAK")) {
        extractor = cv::xfeatures2d::FREAK::create();
    } else if (descriptorType.compare("AKAZE")) {
        extractor = cv::AKAZE::create();
    } else if (descriptorType.compare("SIFT")) {
        extractor = cv::xfeatures2d::SIFT::create();
    } else {
        throw std::invalid_argument("Unknown detector type!");
    }

    // perform feature description
    double t = (double)cv::getTickCount();
    extractor->compute(img, keypoints, descriptors);
    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    cout << descriptorType << " descriptor extraction in " << 1000 * t / 1.0 << " ms" << endl;
}

// Detect keypoints in image using the traditional Shi-Thomasi detector
void detKeypointsShiTomasi(vector<cv::KeyPoint> &keypoints, cv::Mat &img, bool bVis) {
    // compute detector parameters based on image size
    int blockSize = 4;       //  size of an average block for computing a derivative covariation matrix over each pixel neighborhood
    double maxOverlap = 0.0; // max. permissible overlap between two features in %
    double minDistance = (1.0 - maxOverlap) * blockSize;
    int maxCorners = img.rows * img.cols / max(1.0, minDistance); // max. num. of keypoints

    double qualityLevel = 0.01; // minimal accepted quality of image corners
    double k = 0.04;

    // Apply corner detection
    double t = (double)cv::getTickCount();
    vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack(img, corners, maxCorners, qualityLevel, minDistance, cv::Mat(), blockSize, false, k);

    // add corners to result vector
    for (auto it = corners.begin(); it != corners.end(); ++it) {
        cv::KeyPoint newKeyPoint;
        newKeyPoint.pt = cv::Point2f((*it).x, (*it).y);
        newKeyPoint.size = blockSize;
        keypoints.push_back(newKeyPoint);
    }

    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    cout << "Shi-Tomasi detection with n=" << keypoints.size() << " keypoints in " << 1000 * t / 1.0 << " ms" << endl;

    // visualize results
    if (bVis)
    {
        cv::Mat visImage = img.clone();
        cv::drawKeypoints(img, keypoints, visImage, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        string windowName = "Shi-Tomasi Corner Detector Results";
        cv::namedWindow(windowName, 6);
        imshow(windowName, visImage);
        cv::waitKey(0);
    }
}

void detKeypointsHarris(std::vector<cv::KeyPoint> &keypoints, cv::Mat &img, bool bVis) {
    cv::Mat dst, dst_norm, dst_norm_scaled;
    dst = cv::Mat::zeros(img.size(), CV_32FC1);

    // Detector parameters
    int blockSize = 4;
    int apertureSize = 3;
    double k = 0.04;
    int thresh = 200;
    double t = (double)cv::getTickCount();

    cv::cornerHarris(img, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT);
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    cv::convertScaleAbs(dst_norm, dst_norm_scaled);

    for(int j = 0; j < dst_norm.rows ; j++ ) { // y
        for(int i = 0; i < dst_norm.cols; i++ ) { // x
            if((int) dst_norm.at<float>(j,i) > thresh ) {
                cv::KeyPoint newKeyPoint;
                newKeyPoint.pt = cv::Point2f(i, j);
                newKeyPoint.size = 1;
                keypoints.push_back(newKeyPoint);
              }
          }
     }

     t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
     cout << "Harris detection with n=" << keypoints.size() << " keypoints in " << 1000 * t / 1.0 << " ms" << endl;
}

void detKeypointsModern(std::vector<cv::KeyPoint> &keypoints, cv::Mat &img, std::string detectorType, bool bVis) {
    double t = (double)cv::getTickCount();

    if (detectorType.compare("FAST") == 0) {
        cv::Ptr<cv::FastFeatureDetector> FAST = cv::FastFeatureDetector::create();
        FAST->detect(img, keypoints);
    } else if (detectorType.compare("BRISK") == 0) {
        cv::Ptr<cv::BRISK> BRISK = cv::BRISK::create();
        BRISK->detect(img, keypoints);
    } else if (detectorType.compare("ORB") == 0) {
        cv::Ptr<cv::ORB> ORB = cv::ORB::create();
        ORB->detect(img, keypoints);
    } else if (detectorType.compare("AKAZE") == 0) {
        cv::Ptr<cv::AKAZE> AKAZE = cv::AKAZE::create();
        AKAZE->detect(img, keypoints);
    } else if (detectorType.compare("SIFT") == 0) {
        cv::Ptr<cv::xfeatures2d::SIFT> SIFT = cv::xfeatures2d::SIFT::create();
        SIFT->detect(img, keypoints);
    } else {
        throw std::invalid_argument("Unknown detector type!");
    }

    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    cout << detectorType << " detection with n=" << keypoints.size() << " keypoints in " << 1000 * t / 1.0 << " ms" << endl;

}
