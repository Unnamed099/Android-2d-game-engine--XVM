#ifndef RECTF_JNI_H
#define RECTF_JNI_H

/**
 * 纯 C++ 的 RectF（浮点矩形）
 * 对应 android.graphics.RectF，但不依赖 JNI
 */
class RectF {
public:
    float left = 0.0f;
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;

    // ============ 构造 ============
    RectF() {}

    RectF(float left, float top, float right, float bottom)
            : left(left), top(top), right(right), bottom(bottom) {}

    RectF(const RectF& other)
            : left(other.left), top(other.top), right(other.right), bottom(other.bottom) {}

    RectF& operator=(const RectF& other) {
        if (this != &other) {
            left = other.left;
            top = other.top;
            right = other.right;
            bottom = other.bottom;
        }
        return *this;
    }

    // ============ set ============
    void set(float l, float t, float r, float b) {
        left = l;
        top = t;
        right = r;
        bottom = b;
    }

    void set(const RectF& other) {
        left = other.left;
        top = other.top;
        right = other.right;
        bottom = other.bottom;
    }

    void setEmpty() {
        left = top = right = bottom = 0.0f;
    }

    // ============ 尺寸 ============
    float width() const { return right - left; }
    float height() const { return bottom - top; }
    float centerX() const { return (left + right) * 0.5f; }
    float centerY() const { return (top + bottom) * 0.5f; }

    // ============ 判断 ============
    bool isEmpty() const {
        return left >= right || top >= bottom;
    }

    bool contains(float x, float y) const {
        return x >= left && x < right && y >= top && y < bottom;
    }

    bool contains(const RectF& other) const {
        return left <= other.left && top <= other.top &&
               right >= other.right && bottom >= other.bottom;
    }

    bool intersect(const RectF& other) const {
        return !(left >= other.right || right <= other.left ||
                 top >= other.bottom || bottom <= other.top);
    }

    // ============ 偏移 ============
    void offset(float dx, float dy) {
        left += dx;
        top += dy;
        right += dx;
        bottom += dy;
    }

    void offsetTo(float newLeft, float newTop) {
        right = newLeft + width();
        bottom = newTop + height();
        left = newLeft;
        top = newTop;
    }

    // ============ 缩放/扩展 ============
    void inset(float dx, float dy) {
        left += dx;
        top += dy;
        right -= dx;
        bottom -= dy;
    }

    void union_(const RectF& other) {
        if (other.isEmpty()) return;
        if (isEmpty()) {
            set(other);
            return;
        }
        if (left > other.left) left = other.left;
        if (top > other.top) top = other.top;
        if (right < other.right) right = other.right;
        if (bottom < other.bottom) bottom = other.bottom;
    }

    // ============ 比较 ============
    bool operator==(const RectF& other) const {
        return left == other.left && top == other.top &&
               right == other.right && bottom == other.bottom;
    }

    bool operator!=(const RectF& other) const {
        return !(*this == other);
    }
};

#endif // RECTF_JNI_H