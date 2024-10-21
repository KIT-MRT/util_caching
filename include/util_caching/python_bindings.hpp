#include <chrono>

#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "cache.hpp"

namespace util_caching::python_api {

namespace py = pybind11;

namespace number_based {
namespace internal {

/*!
 * \brief Bind the comparison policies to the Cache class
 *
 * This function binds the comparison policies to the Cache class. The policies
 * are passed as variadic template arguments. The function overloads the
 * `cached` function for each policy.
 */
template <typename CacheT, typename NumberT, typename... ComparisonPolicyTs>
void bindPolicies(py::class_<CacheT, std::shared_ptr<CacheT>> &cacheClass) {
  (cacheClass.def(
       "cached",
       [](CacheT &self, const NumberT &key, const ComparisonPolicyTs &policy) {
         return self.template cached<ComparisonPolicyTs>(key, policy);
       },
       py::arg("key"), py::arg("policy")),
   ...);
}
} // namespace internal

/*!
 * \brief Bind the ApproximateNumber policy
 *
 * This function adds bindings for the ApproximateNumber policy to the given
 * python module under the given name.
 */
template <typename NumberT>
void bindApproximatePolicy(py::module &module,
                           const std::string &name = "ApproximateNumber") {
  using ApproximateNumberT = policies::ApproximateNumber<NumberT>;
  py::class_<ApproximateNumberT, std::shared_ptr<ApproximateNumberT>>(
      module, name.c_str())
      .def(py::init<NumberT>(), py::arg("threshold"))
      .def("__call__", &ApproximateNumberT::operator(), "Compare two numbers");
}

/*!
 * \brief Bindings for a Cache that is based on number comparisons
 *
 * This function binds the Cache class for a specific number-based key type
 * (NumberT) and value type (ValueT). Optionally, add a list of comparison
 * policies to the list of template parameters. The `cached` function will be
 * overloaded for each one of them. Call this function once inside
 * PYBIND11_MODULE macro to create the bindings for the Cache class.
 */
template <typename NumberT, typename ValueT, typename... ComparisonPolicies>
void bindCache(py::module &module) {
  using CacheT = Cache<NumberT, ValueT>;
  py::class_<CacheT, std::shared_ptr<CacheT>> cache(module, "Cache");
  cache
      .def(py::init<>())
      // We cannot pass template parameters to python functions, therefore we
      // need to explicitly bind all instantiations to different python
      // functions. We need to use the lambdas here to handle the seconds
      // argument, defining the comparison policy.
      .def(
          "cached",
          [](CacheT &self, const NumberT &key) {
            return self.template cached<std::equal_to<NumberT>>(key);
          },
          py::arg("key"))
      .def("cache", &CacheT::cache, py::arg("key"), py::arg("value"))
      .def("reset", &CacheT::reset);

  internal::bindPolicies<CacheT, NumberT, ComparisonPolicies...>(cache);
}

} // namespace number_based

namespace time_based {
namespace internal {

/*!
 * \brief Bind the comparison policies to the Cache class
 *
 * This function binds the comparison policies to the Cache class. The policies
 * are passed as variadic template arguments. The function overloads the
 * `cached` function for each policy.
 */
template <typename CacheT, typename TimeT, typename... ComparisonPolicyTs>
void bindPolicies(py::class_<CacheT, std::shared_ptr<CacheT>> &cache) {
  (cache.def(
       "cached",
       [](CacheT &self, const TimeT &key, const ComparisonPolicyTs &policy) {
         return self.template cached<ComparisonPolicyTs>(key, policy);
       },
       py::arg("key"), py::arg("policy")),
   ...);
}
} // namespace internal

/*!
 * \brief Bind the ApproximateTime policy
 *
 * This function adds bindings for the ApproximateTime policy to the given
 * python module under the given name.
 */
template <typename TimeT, typename ThresholdTimeUnitT>
void bindApproximatePolicy(py::module &module,
                           const std::string &name = "ApproximateTime") {
  using ApproximateTimeT = policies::ApproximateTime<TimeT, ThresholdTimeUnitT>;
  py::class_<ApproximateTimeT, std::shared_ptr<ApproximateTimeT>>(module,
                                                                  name.c_str())
      .def(py::init<double>(), py::arg("threshold"))
      .def("__call__", &ApproximateTimeT::operator(),
           "Compare two time points");
}

/*!
 * \brief Bindings for a Cache that is based on time comparisons.
 *
 * This function binds the Cache class for a specific time-based key type
 * (TimeT) and value type (ValueT). Optionally, add a list of comparison
 * policies to the list of template parameters. The `cached` function will be
 * overloaded for each one of them. Call this function once inside
 * PYBIND11_MODULE macro to create the bindings for the Cache class.
 */
template <typename TimeT, typename ValueT, typename... ComparisonPolicyTs>
void bindCache(py::module &module) {
  using CacheT = Cache<TimeT, ValueT>;

  py::class_<CacheT, std::shared_ptr<CacheT>> cache(module, "Cache");
  cache.def(py::init<>())
      .def(
          "cached",
          [](CacheT &self, const TimeT &key) {
            return self.template cached<std::equal_to<TimeT>>(key);
          },
          py::arg("key"))
      .def("cache", &CacheT::cache, py::arg("key"), py::arg("value"))
      .def("reset", &CacheT::reset);

  internal::bindPolicies<CacheT, TimeT, ComparisonPolicyTs...>(cache);
}

} // namespace time_based
} // namespace util_caching::python_api
