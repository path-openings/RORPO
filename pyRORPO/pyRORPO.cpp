#include "pyRORPO.hpp"

#include "bind_RORPO.hpp"
#include "bind_RORPO_multiscale.hpp"
#include "bind_RPO.hpp"
#include "bind_PO.hpp"

using namespace pyRORPO;

#define BINDINGS(type) \
    RORPO_MULTISCALE_BINDING(type); \
    RORPO_BINDING(type); \
    RPO_BINDING(type); \
    PO_BINDING(type); \

PYBIND11_MODULE(pyRORPO, m) {
    m.doc() = "pybind11 RORPO plugin"; // optional module docstring

    BINDINGS(int8_t);
    BINDINGS(int16_t);
    BINDINGS(int32_t);
    BINDINGS(int64_t);

    BINDINGS(uint8_t);
    BINDINGS(uint16_t);
    BINDINGS(uint32_t);
    BINDINGS(uint64_t);

    BINDINGS(float);
    BINDINGS(double );
    BINDINGS(long double);

    RORPO_MULTISCALE_FROMFILE_BINDING(RORPO_multiscale_binding_fromPath) \
    RORPO_FROMFILE_BINDING() \

}