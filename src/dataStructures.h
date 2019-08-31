#ifndef dataStructures_h
#define dataStructures_h

#include <vector>
#include <opencv2/core.hpp>
#include <deque>


struct DataFrame { // represents the available sensor information at the same time instance
    cv::Mat cameraImg; // camera image

    std::vector<cv::KeyPoint> keypoints; // 2D keypoints within camera image
    cv::Mat descriptors; // keypoint descriptors
    std::vector<cv::DMatch> kptMatches; // keypoint matches between previous and current frame
};


template <class T>
class RingBuffer {
public:
    RingBuffer(size_t size) {
        size_t max_size_ = size;
        std::deque<T> _buffer;
    }

    size_t size() {
        return _buffer.size();
    }

    void push_back(T item) {
        _buffer.push_back(item);
        if (_buffer.size() > max_size_) {
            _buffer.pop_front();
        }
    }

    T get() {
        return _buffer.pop_front();
    }

    typename std::deque<T>::iterator end() {
        return _buffer.end();
    }

private:
    size_t max_size_;
    std::deque<T> _buffer;
};


#endif /* dataStructures_h */
