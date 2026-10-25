#ifndef LMS_LATE_PENALTY_H
#define LMS_LATE_PENALTY_H
#include <algorithm>

namespace lms {
class LatePenalty {
public:
    virtual ~LatePenalty() = default;
    virtual double apply(double mark, double maximum, int lateDays) const = 0;
};
class SchoolLatePenalty final : public LatePenalty {
public:
    double apply(double mark, double maximum, int lateDays) const override {
        if (lateDays <= 0) return mark;
        if (lateDays > 3) return 0;
        return std::max(0.0, mark - maximum * 0.10 * lateDays);
    }
};
}
#endif
