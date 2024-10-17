#include <pybind11/pybind11.h>

#include "cache.hpp"
#include "python_bindings.hpp"

namespace py = pybind11;

using namespace util_caching;

PYBIND11_MODULE(util_caching_py, mainModule) {
    py::module numberBased = mainModule.def_submodule("number_based");
    python_api::bindNumberBasedCache<double, double>(numberBased);

    py::module timeBased = mainModule.def_submodule("time_based");
    python_api::bindTimeBasedCache<Time, double, std::chrono::milliseconds>(timeBased);
}
