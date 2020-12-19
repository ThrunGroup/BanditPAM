/**
 * @file kmeds_pywrapper.cpp
 * @date 2020-06-10
 *
 * Creates the Python bindings for the C++ code that
 * allows it to be called in Python.
 *
 */
#include <armadillo>
#include <carma/carma.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "kmedoids_ucb.hpp"

namespace py = pybind11;

/**
 *  \brief Python wrapper for KMedoids class.
 *
 *  KMedsWrapper class. Is the Python wrapper generated by Pybind that allows for calling
 *  the C++ code in Python.
 */
class KMedsWrapper : public KMedoids {
public:
    using KMedoids::KMedoids;

    /**
     * \brief Python binding for fitting a KMedoids object to the
     *
     * This is the primary function of the KMedoids module: this finds the build and swap
     * medoids for the desired data
     *
     * @param inputData Input data to find the medoids of
     * @param loss The loss function used during medoid computation
     * @param k The number of medoids to compute
     * @param logFilename The name of the outputted log file
     */
    void fitPython(py::array_t<double> inputData, std::string loss, int k, std::string logFilename) {
        KMedoids::setNMedoids(k);
        KMedoids::setLogFilename(logFilename);
        KMedoids::fit(carma::arr_to_mat<double>(inputData), loss);
    }

    /**
     *  \brief Returns the final medoids
     *
     *  Returns as a numpy array the final medoids at the end of the SWAP step
     *  after KMedoids::fit has been called.
     */
    py::array_t<double> getMedoidsFinalPython() {
        return carma::row_to_arr<double>(KMedoids::getMedoidsFinal()).squeeze();
    }

    /**
     *  \brief Returns the build medoids
     *
     *  Returns as a numpy array the build medoids at the end of the BUILD step
     *  after KMedoids::fit has been called.
     */
    py::array_t<double> getMedoidsBuildPython() {
        return carma::row_to_arr<double>(KMedoids::getMedoidsBuild()).squeeze();
    }

    /**
     *  \brief Returns the medoid assignments for each datapoint
     *
     *  Returns as a numpy array the medoid each input datapoint is assigned to
     *  after KMedoids::fit is called and the final medoids have been identified
     */
    py::array_t<double> getLabelsPython() {
        return carma::row_to_arr<double>(KMedoids::getLabels()).squeeze();
    }

    /**
     *  \brief Returns the number of swap steps
     *
     *  Returns the number of SWAP steps completed during the last call to
     *  KMedoids::fit
     */
    int getStepsPython() {
        return KMedoids::getSteps();
    }
};

PYBIND11_MODULE(BanditPAM, m) {
    m.doc() = "BanditPAM Python library, implemented in C++";
    py::class_<KMedsWrapper>(m, "KMedoids")
            .def(py::init<int, std::string, int, int, std::string, bool>(),
                 py::arg("n_medoids") = 5,
                 py::arg("algorithm") = "BanditPAM",
                 py::arg("verbosity") = 0,
                 py::arg("maxIter") = 1000,
                 py::arg("logFilename") = "KMedoidsLogfile",
                 py::arg("cache") = true
            )
            .def_property("n_medoids", &KMedsWrapper::getNMedoids, &KMedsWrapper::setNMedoids)
            .def_property("algorithm", &KMedsWrapper::getAlgorithm, &KMedsWrapper::setAlgorithm)
            .def_property("verbosity", &KMedsWrapper::getVerbosity, &KMedsWrapper::setVerbosity)
            .def_property("maxIter", &KMedsWrapper::getMaxIter, &KMedsWrapper::setMaxIter)
            .def_property("logFilename", &KMedsWrapper::getLogfileName, &KMedsWrapper::setLogFilename)
            .def_property_readonly("final_medoids", &KMedsWrapper::getMedoidsFinalPython)
            .def_property_readonly("build_medoids", &KMedsWrapper::getMedoidsBuildPython)
            .def_property_readonly("labels", &KMedsWrapper::getLabelsPython)
            .def_property_readonly("steps", &KMedsWrapper::getStepsPython)
            .def("fit", &KMedsWrapper::fitPython);
}
