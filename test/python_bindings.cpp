#include <pybind11/pybind11.h>

#include "cache.hpp"
#include "python_bindings.hpp"
#include "types.hpp"

namespace py = pybind11;

using namespace util_caching;

/*!
 * \brief A policy that always returns true
 *
 * Custom policies have to be defined in C++ and then bound to Python.
 * To overload the `cache` function, the policy has to be passed as a template parameter to the `bindCache` function.
 */
struct SomePolicyWithoutParams {
    SomePolicyWithoutParams() = default;
    bool operator()(const Time& /*lhs*/, const Time& /*rhs*/) const {
        return true;
    }
};

/*!
 * \brief The python module definition that allows running python unit tests equivalent to the native C++ ones.
 */
PYBIND11_MODULE(util_caching_py, mainModule) {
    // Just some aliases to make the code more readable
    using ApproximateNumberT = policies::ApproximateNumber<double>;
    using ApproximateTimeT = policies::ApproximateTime<Time, std::chrono::milliseconds>;
    using ApproximateTimeSecondsT = policies::ApproximateTime<Time, std::chrono::seconds>;

    // Since we want to use this policy in python, we need to be able to instatiate it there
    py::class_<SomePolicyWithoutParams, std::shared_ptr<SomePolicyWithoutParams>>(mainModule, "SomePolicyWithoutParams")
        .def(py::init<>())
        .def("__call__", &SomePolicyWithoutParams::operator());

    // Adding a submodule is optional but a good way to structure the bindings
    py::module numberBased = mainModule.def_submodule("number_based");
    // If we want to use a policy, we need to bind it. For the builtin policies, we can use this convenience function.
    python_api::number_based::bindApproximatePolicy<double>(numberBased);
    // The core binding, the cache class itself.
    python_api::number_based::bindCache<double,            // KeyType
                                        double,            // Value type
                                        ApproximateNumberT // Optionally, a list of comparison policies, each one will
                                                           // overload the `cached` function
                                        >(numberBased);

    // Same as above, but for the time-based cache
    py::module timeBased = mainModule.def_submodule("time_based");
    // We can bind the builtin comparison policy for different time units but then we have to name them differently
    python_api::time_based::bindApproximatePolicy<Time, std::chrono::milliseconds>(timeBased, "ApproximateTime");
    python_api::time_based::bindApproximatePolicy<Time, std::chrono::seconds>(timeBased, "ApproximateTimeSeconds");
    // The core binding, the cache class itself.
    python_api::time_based::bindCache<Time,             // Key type
                                      double,           // Value type
                                      ApproximateTimeT, // A list of all comparison policies we intend to use
                                      ApproximateTimeSecondsT,
                                      SomePolicyWithoutParams>(timeBased);
}
