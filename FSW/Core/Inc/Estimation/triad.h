#pragma once

#include "Math/math_types.h"

#include <Eigen/Geometry>
#include <cmath>

namespace adcs::est
{
class Triad final
{
public:
    math::Vec3 v1{0.0F, 0.0F, 1.0F};
    math::Vec3 v2{1.0F, 0.0F, 0.0F};

    math::Mat3 estimateDCM(const math::Vec3& w1,
                           const math::Vec3& w2) const noexcept
    {
        return coreEstimate(w1, w2);
    }

    math::quat estimateQuat(const math::Vec3& w1,
                            const math::Vec3& w2) const noexcept
    {
        return math::quat(coreEstimate(w1, w2)).normalized();
    }

private:

    static bool normalized(const math::Vec3& input, math::Vec3& output) noexcept
    {
        const math::Scalar norm_sq = input.squaredNorm();
        if (norm_sq <= 1.0E-12F)
        {
            return false;
        }

        output = input / std::sqrt(norm_sq);
        return true;
    }

    static bool makeTriad(const math::Vec3& first,
                          const math::Vec3& second,
                          math::Mat3& triad) noexcept
    {
        math::Vec3 t1{};
        math::Vec3 second_unit{};
        if ((!normalized(first, t1)) || (!normalized(second, second_unit)))
        {
            return false;
        }

        math::Vec3 t2{};
        if (!normalized(t1.cross(second_unit), t2))
        {
            return false;
        }

        const math::Vec3 t3 = t1.cross(t2);
        triad << t1, t2, t3;
        return true;
    }

    math::Mat3 coreEstimate(const math::Vec3& w1,
                            const math::Vec3& w2) const noexcept
    {
        math::Mat3 body_triad{};
        math::Mat3 reference_triad{};

        if ((!makeTriad(w1, w2, body_triad)) || (!makeTriad(v1, v2, reference_triad)))
        {
            return math::Mat3::Identity();
        }

        return reference_triad * body_triad.transpose();
    }
};
}
