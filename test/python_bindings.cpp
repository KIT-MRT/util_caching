#include <pybind11/pybind11.h>

#include "cache.hpp"
#include "python_bindings.hpp"
#include "types.hpp"

namespace py = pybind11;

using namespace util_caching;

struct SomePolicyWithoutParams {
    SomePolicyWithoutParams() = default;
    bool operator()(const Time& /*lhs*/, const Time& /*rhs*/) const {
        return true;
    }
};

PYBIND11_MODULE(util_caching_py, mainModule) {
    using ApproximateNumberT = policies::ApproximateNumber<double>;
    using ApproximateTimeT = policies::ApproximateTime<Time, std::chrono::milliseconds>;
    using ApproximateTimeSecondsT = policies::ApproximateTime<Time, std::chrono::seconds>;

    py::class_<SomePolicyWithoutParams, std::shared_ptr<SomePolicyWithoutParams>>(mainModule, "SomePolicyWithoutParams")
        .def(py::init<>())
        .def("__call__", &SomePolicyWithoutParams::operator());

    py::module numberBased = mainModule.def_submodule("number_based");
    python_api::number_based::bindApproximatePolicy<double>(numberBased);
    python_api::number_based::bindCache<double, double, ApproximateNumberT>(numberBased);

    py::module timeBased = mainModule.def_submodule("time_based");
    python_api::time_based::bindApproximatePolicy<Time, std::chrono::milliseconds>(timeBased, "ApproximateTime");
    python_api::time_based::bindApproximatePolicy<Time, std::chrono::seconds>(timeBased, "ApproximateTimeSeconds");
    python_api::time_based::bindCache<Time, double, ApproximateTimeT, ApproximateTimeSecondsT, SomePolicyWithoutParams>(
        timeBased);
}
