#pragma once

#include <cmath>
#include <functional>
#include <optional>
#include "types.hpp"

namespace util_caching {
namespace policies {

// The ApproximateTime struct is one example that can be used as key for the Cache
template <typename TimeType = Time, typename TimeUnitType = std::chrono::milliseconds>
struct ApproximateTime {
    explicit ApproximateTime(const double threshold) : threshold_{threshold} {
    }
    // defines the policy that two time points is close enough
    bool operator()(const TimeType& lhs, const TimeType& rhs) const {
        const double diff = std::chrono::duration_cast<TimeUnitType>(lhs - rhs).count();
        return std::fabs(diff) <= threshold_;
    }

private:
    double threshold_; // threshold in TimeUnitType
};

template <typename NumberType>
struct ApproximateNumber {
    explicit ApproximateNumber(const NumberType threshold) : threshold_{threshold} {
    }
    // defines the policy that two numbers is close enough
    bool operator()(const NumberType& lhs, const NumberType& rhs) const {
        return std::fabs(lhs - rhs) <= threshold_;
    }

private:
    NumberType threshold_; // threshold in TimeUnitType
};

} // namespace policies

/*!
 * \brief The Cache class is one templated class for caching computed values that can be accessed by
 *        giving the same or close enough key
 *
 * In general, the value and key that to be cached can be any type.
 */
template <typename KeyType, typename ValueType>
class Cache {
public:
    Cache() = default;

    /*!
     * \brief Check if there is already value cached that is close enough to the key according the ComparisonPolicy
     *
     * \return  std::nullopt if not cached, otherwise return the cached value
     */
    template <class ComparisonPolicy = std::equal_to<KeyType>>
    std::optional<ValueType> cached(const KeyType& key, const ComparisonPolicy& equal = ComparisonPolicy()) {
        if (value_ && key_ && equal(key, *key_)) {
            return value_;
        }
        return std::nullopt;
    }

    /*!
     * \brief Cache the value and key
     *
     * \param value  The value that is to be cached
     * \param key  The key that is correlated to the value
     */
    void cache(const KeyType& key, const ValueType& value) {
        value_ = value;
        key_ = key;
    }

    /*!
     * @brief Reset the cached value and key
     */
    void reset() {
        value_.reset();
        key_.reset();
    }

private:
    std::optional<ValueType> value_;
    std::optional<KeyType> key_;
};

} // namespace util_caching
