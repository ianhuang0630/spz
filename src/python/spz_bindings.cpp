#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "../cc/load-spz.h"

namespace py = pybind11;

PYBIND11_MODULE(spz, m) {
    m.doc() = "Python bindings for SPZ - 3D Gaussian Splat compression library";

    // Enums
    py::enum_<spz::CoordinateSystem>(m, "CoordinateSystem")
        .value("UNSPECIFIED", spz::CoordinateSystem::UNSPECIFIED)
        .value("LDB", spz::CoordinateSystem::LDB)
        .value("RDB", spz::CoordinateSystem::RDB)
        .value("LUB", spz::CoordinateSystem::LUB)
        .value("RUB", spz::CoordinateSystem::RUB)
        .value("LDF", spz::CoordinateSystem::LDF)
        .value("RDF", spz::CoordinateSystem::RDF)
        .value("LUF", spz::CoordinateSystem::LUF)
        .value("RUF", spz::CoordinateSystem::RUF);

    // Options classes
    py::class_<spz::PackOptions>(m, "PackOptions")
        .def(py::init<>())
        .def_readwrite("from_", &spz::PackOptions::from);  // Changed from "from" to "from_"

    py::class_<spz::UnpackOptions>(m, "UnpackOptions")
        .def(py::init<>())
        .def_readwrite("to", &spz::UnpackOptions::to);

    // GaussianCloud class
    py::class_<spz::GaussianCloud>(m, "GaussianCloud")
        .def(py::init<>())
        .def_readwrite("numPoints", &spz::GaussianCloud::numPoints)
        .def_readwrite("shDegree", &spz::GaussianCloud::shDegree)
        .def_readwrite("antialiased", &spz::GaussianCloud::antialiased)
        .def_readwrite("positions", &spz::GaussianCloud::positions)
        .def_readwrite("scales", &spz::GaussianCloud::scales)
        .def_readwrite("rotations", &spz::GaussianCloud::rotations)
        .def_readwrite("alphas", &spz::GaussianCloud::alphas)
        .def_readwrite("colors", &spz::GaussianCloud::colors)
        .def_readwrite("sh", &spz::GaussianCloud::sh);

    // Main functions
    m.def("save_spz", 
        [](const spz::GaussianCloud& gaussians, const spz::PackOptions& options, const std::string& filename) {
            return spz::saveSpz(gaussians, options, filename);
        },
        "Save Gaussian splat to SPZ file",
        py::arg("gaussians"), py::arg("options"), py::arg("filename"));

    m.def("load_spz", 
        [](const std::string& filename, const spz::UnpackOptions& options) {
            return spz::loadSpz(filename, options);
        },
        "Load Gaussian splat from SPZ file",
        py::arg("filename"), py::arg("options"));

    m.def("save_spz_to_bytes", 
        [](const spz::GaussianCloud& gaussians, const spz::PackOptions& options) {
            std::vector<uint8_t> output;
            bool success = spz::saveSpz(gaussians, options, &output);
            if (!success) {
                throw std::runtime_error("Failed to save SPZ to bytes");
            }
            return output;
        },
        "Save Gaussian splat to bytes",
        py::arg("gaussians"), py::arg("options"));

    m.def("load_spz_from_bytes", 
        [](const std::vector<uint8_t>& data, const spz::UnpackOptions& options) {
            return spz::loadSpz(data, options);
        },
        "Load Gaussian splat from bytes",
        py::arg("data"), py::arg("options"));

    m.def("save_splat_to_ply", 
        [](const spz::GaussianCloud& gaussians, const spz::PackOptions& options, const std::string& filename) {
            return spz::saveSplatToPly(gaussians, options, filename);
        },
        "Save Gaussian splat to PLY file",
        py::arg("gaussians"), py::arg("options"), py::arg("filename"));

    m.def("load_splat_from_ply", 
        [](const std::string& filename, const spz::UnpackOptions& options) {
            return spz::loadSplatFromPly(filename, options);
        },
        "Load Gaussian splat from PLY file",
        py::arg("filename"), py::arg("options"));
}