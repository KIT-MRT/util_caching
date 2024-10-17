#include <chrono>

#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "cache.hpp"

namespace util_caching::python_api {

namespace py = pybind11;

/*!
 * \brief Bindings for a Cache that is based on number comparisons
 *
 * This function binds the Cache class for a specific number-based key type (NumberT) and value type (ValueT).
 * Call this function once inside PYBIND11_MODULE macro to create a Python module with the bindings.
 */
template <typename NumberT, typename ValueT, typename ThresholdNumberT = NumberT>
void bindNumberBasedCache(py::module& module) {
    using CacheT = Cache<NumberT, ValueT>;
    using ApproximateNumberT = policies::ApproximateNumber<ThresholdNumberT>;

    py::class_<ApproximateNumberT, std::shared_ptr<ApproximateNumberT>>(module, "ApproximateNumber")
        .def(py::init<ThresholdNumberT>(), py::arg("threshold"))
        .def("__call__", &ApproximateNumberT::operator(), "Compare two values");

    py::class_<CacheT, std::shared_ptr<CacheT>>(module, "Cache")
        .def(py::init<>())
        // We cannot pass template parameters to python functions, therefore we need to explicitly bind all
        // instantiations to different python functions.
        // We need to use the lambdas here to handle the seconds argument, defining the comparison policy.
        .def(
            "cached",
            [](CacheT& self, const NumberT& key) { return self.template cached<std::equal_to<NumberT>>(key); },
            py::arg("key"))
        // Approximate match: using ApproximateNumber policy
        .def(
            "cached",
            [](CacheT& self, const NumberT& key, const ApproximateNumberT& policy) {
                return self.template cached<ApproximateNumberT>(key, policy);
            },
            py::arg("key"),
            py::arg("policy"),
            "Check if the value is approximately cached based on the given policy")
        .def("cache", &CacheT::cache, py::arg("key"), py::arg("value"))
        .def("reset", &CacheT::reset);
}

/*!
 * \brief Bindings for a Cache that is based on time comparisons
 *
 * This function binds the Cache class for a specific time-based key type (TimeT) and value type (ValueT).
 * Call this function once inside PYBIND11_MODULE macro to create a Python module with the bindings.
 */
template <typename TimeT, typename ValueT, typename ThresholdTimeUnitT = std::chrono::milliseconds>
void bindTimeBasedCache(py::module& module) {
    using CacheT = Cache<TimeT, ValueT>;
    using ApproximateTimeT = policies::ApproximateTime<TimeT, ThresholdTimeUnitT>;

    py::class_<ApproximateTimeT>(module, "ApproximateTime")
        .def(py::init<double>(), py::arg("threshold"))
        .def("__call__", &ApproximateTimeT::operator(), "Compare two time points");

    py::class_<CacheT, std::shared_ptr<CacheT>>(module, "Cache")
        .def(py::init<>())
        .def(
            "cached",
            [](CacheT& self, const TimeT& key) { return self.template cached<std::equal_to<TimeT>>(key); },
            py::arg("key"))
        .def(
            "cached",
            [](CacheT& self, const TimeT& key, const ApproximateTimeT& policy) {
                return self.template cached<ApproximateTimeT>(key, policy);
            },
            py::arg("key"),
            py::arg("policy"),
            "Check if the value is approximately cached based on the given policy")
        .def("cache", &CacheT::cache, py::arg("key"), py::arg("value"))
        .def("reset", &CacheT::reset);
}

} // namespace util_caching::python_api
