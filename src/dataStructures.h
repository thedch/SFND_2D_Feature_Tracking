#ifndef dataStructures_h
#define dataStructures_h

#include <vector>
#include <opencv2/core.hpp>


struct DataFrame { // represents the available sensor information at the same time instance
    cv::Mat cameraImg; // camera image

    std::vector<cv::KeyPoint> keypoints; // 2D keypoints within camera image
    cv::Mat descriptors; // keypoint descriptors
    std::vector<cv::DMatch> kptMatches; // keypoint matches between previous and current frame
};

template <class T>
class RingBuffer {
public:
    explicit RingBuffer(size_t size) :
        buf_(std::unique_ptr<T[]>(new T[size])),
        max_size_(size)
        { /* empty */ }

    void reset() {
        head_ = tail_;
        full_ = false;
    }

    bool empty() {
        return (!full_ && (head_ == tail_));
    }

    bool full() {
        return full_;
    }

    size_t capacity() const {
        return max_size_;
    }

    void put(T item) {
        buf_[head_] = item;

        if (full_) {
            tail_ = (tail_ + 1) % max_size_;
        }

        head_ = (head_ + 1) % max_size_;
        full_ = (head_ == tail_);
    }

    T get() {
        if (empty()) {
            return T(); // or throw error..?
        }

        auto val = buf_[tail_];
        full_ = false;
        tail_ = (tail_ + 1) % max_size_;

        return val;
    }

private:
    size_t head_ = 0;
    size_t tail_ = 0;
    const size_t max_size_;
    bool full_ = false;
    std::unique_ptr<T[]> buf_;
};


#endif /* dataStructures_h */
