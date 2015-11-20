/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @brief wraps Rot3 class to python
 * @author Andrew Melim
 * @author Ellon Paiva Mendes (LAAS-CNRS)
 **/

#include <boost/python.hpp>
#include "gtsam/geometry/Rot3.h"

using namespace boost::python;
using namespace gtsam;

// Prototypes used to perform overloading
// See: http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html
gtsam::Rot3  (*AxisAngle_0)(const Vector3&, double) = &Rot3::AxisAngle;
gtsam::Rot3  (*AxisAngle_1)(const gtsam::Point3&, double) = &Rot3::AxisAngle;
gtsam::Rot3  (*Rodrigues_0)(const Vector3&) = &Rot3::Rodrigues;
gtsam::Rot3  (*Rodrigues_1)(double, double, double) = &Rot3::Rodrigues;
gtsam::Rot3  (*RzRyRx_0)(double, double, double) = &Rot3::RzRyRx;
gtsam::Rot3  (*RzRyRx_1)(const Vector&) = &Rot3::RzRyRx;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(print_overloads, Rot3::print, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(equals_overloads, Rot3::equals, 1, 2)

void exportRot3(){

  class_<Rot3>("Rot3")
    .def(init<>())
    .def(init<Point3,Point3,Point3>())
    .def(init<double,double,double,double,double,double,double,double,double>())
    .def(init<const Matrix3 &>())
    .def(init<const Matrix &>())
    .def("AxisAngle", AxisAngle_0)
    .def("AxisAngle", AxisAngle_1)
    .staticmethod("AxisAngle")
    .def("Expmap", &Rot3::Expmap)
    .staticmethod("Expmap")
    .def("ExpmapDerivative", &Rot3::ExpmapDerivative)
    .staticmethod("ExpmapDerivative")
    .def("Logmap", &Rot3::Logmap)
    .staticmethod("Logmap")
    .def("LogmapDerivative", &Rot3::LogmapDerivative)
    .staticmethod("LogmapDerivative")
    .def("Rodrigues", Rodrigues_0)
    .def("Rodrigues", Rodrigues_1)
    .staticmethod("Rodrigues")
    .def("Rx", &Rot3::Rx)
    .staticmethod("Rx")
    .def("Ry", &Rot3::Ry)
    .staticmethod("Ry")
    .def("Rz", &Rot3::Rz)
    .staticmethod("Rz")
    .def("RzRyRx", RzRyRx_0)
    .def("RzRyRx", RzRyRx_1)
    .staticmethod("RzRyRx")
    .def("identity", &Rot3::identity)
    .staticmethod("identity")
    .def("AdjointMap", &Rot3::AdjointMap)
    .def("column", &Rot3::column)
    .def("conjugate", &Rot3::conjugate)
    .def("equals", &Rot3::equals, equals_overloads(args("q","tol")))
    .def("localCayley", &Rot3::localCayley)
    .def("matrix", &Rot3::matrix)
    .def("print", &Rot3::print, print_overloads(args("s")))
    .def("r1", &Rot3::r1)
    .def("r2", &Rot3::r2)
    .def("r3", &Rot3::r3)
    .def("retractCayley", &Rot3::retractCayley)
    .def("rpy", &Rot3::rpy)
    .def("slerp", &Rot3::slerp)
    .def("transpose", &Rot3::transpose)
    .def("xyz", &Rot3::xyz)
    .def(self * self)
    .def(self * other<Point3>())
    .def(self * other<Unit3>())
    .def(self_ns::str(self)) // __str__
    .def(repr(self))         // __repr__
  ;

}