#include <pybind11/pybind11.h>

#include "cache.hpp"
#include "python_bindings.hpp"
#include "types.hpp"

namespace py = pybind11;

using namespace util_caching;

PYBIND11_MODULE(util_caching_py, mainModule) {
    py::module numberBased = mainModule.def_submodule("number_based");
    python_api::number_based::bindApproximatePolicy<double>(numberBased);
    python_api::number_based::bindCache<double, double, policies::ApproximateNumber<double>>(numberBased);

    py::module timeBased = mainModule.def_submodule("time_based");
    python_api::time_based::bindApproximatePolicy<Time, std::chrono::milliseconds>(timeBased);
    python_api::time_based::bindCache<Time, double, policies::ApproximateTime<Time, std::chrono::milliseconds>>(
        timeBased);
}
