#include <utility>
#include <algorithm>
namespace cvhelper {
typedef struct cv_rect_t {
    int left;
    int top;
    int right;
    int bottom;
} cv_rect_t;

bool oneInOne(const cv_rect_t& A, const cv_rect_t& B) {
	float xx1 = std::min(A.left, B.left);
	float yy1 = std::min(A.top, B.top);
	float xx2 = std::max(A.right, B.right);
	float yy2 = std::max(A.bottom, B.bottom);
	return (xx1 == A.left && yy1 == A.top && xx2 == A.right && yy2 == A.bottom) ||
			(xx1 == B.left && yy1 == B.top && xx2 == B.right && yy2 == B.bottom);
}

float IoU(const cv_rect_t& A, const cv_rect_t& B) {
	float xx1 = std::max(A.left, B.left);
	float yy1 = std::max(A.top, B.top);
	float xx2 = std::min(A.right, B.right);
	float yy2 = std::min(A.bottom, B.bottom);

	float inter  = std::max(0.0f, xx2 - xx1) * std::max(0.0f, yy2 - yy1);
	float a_area = std::abs(A.right - A.left) * std::abs(A.bottom - A.top);
	float b_area = std::abs(B.right - B.left) * std::abs(B.bottom - B.top);

	return inter / (a_area + b_area - inter);
}

}