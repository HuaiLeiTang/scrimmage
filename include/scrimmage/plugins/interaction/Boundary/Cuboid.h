/*!
 * @file
 *
 * @section LICENSE
 *
 * Copyright (C) 2017 by the Georgia Tech Research Institute (GTRI)
 *
 * This file is part of SCRIMMAGE.
 *
 *   SCRIMMAGE is free software: you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the
 *   Free Software Foundation, either version 3 of the License, or (at your
 *   option) any later version.
 *
 *   SCRIMMAGE is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *   License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with SCRIMMAGE.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Kevin DeMarco <kevin.demarco@gtri.gatech.edu>
 * @author Eric Squires <eric.squires@gtri.gatech.edu>
 * @date 31 July 2017
 * @version 0.1.0
 * @brief Brief file description.
 * @section DESCRIPTION
 * A Long description goes here.
 *
 */

#ifndef INCLUDE_SCRIMMAGE_PLUGINS_INTERACTION_BOUNDARY_CUBOID_H_
#define INCLUDE_SCRIMMAGE_PLUGINS_INTERACTION_BOUNDARY_CUBOID_H_

#include <scrimmage/plugins/interaction/Boundary/BoundaryBase.h>

#include <Eigen/Dense>

#include <iostream>
#include <vector>

using std::cout;
using std::endl;

namespace scrimmage {
namespace interaction {

class Cuboid : public BoundaryBase {
 public:
    Cuboid() {
    }

    void compute_dots() {
        // Source: http://math.stackexchange.com/questions/1472049/check-if-a-point-is-inside-a-rectangular-shaped-area-3d
        u = points_[0] - points_[1];
        v = points_[0] - points_[3];
        w = points_[0] - points_[4];

        u_dot_P0 = u.dot(points_[0]);
        u_dot_P1 = u.dot(points_[1]);
        v_dot_P0 = v.dot(points_[0]);
        v_dot_P3 = v.dot(points_[3]);
        w_dot_P0 = w.dot(points_[0]);
        w_dot_P4 = w.dot(points_[4]);
    }

    virtual bool contains(Eigen::Vector3d p) {
        double u_dot_p = u.dot(p);
        double v_dot_p = v.dot(p);
        double w_dot_p = w.dot(p);

        if ( (u_dot_P0 > u_dot_p) && (u_dot_p > u_dot_P1) &&
             (v_dot_P0 > v_dot_p) && (v_dot_p > v_dot_P3) &&
             (w_dot_P0 > w_dot_p) && (w_dot_p > w_dot_P4)) {
            return true;
        }
        return false;
    }

    void set_points(std::vector<Eigen::Vector3d> &points) {
        points_ = points;
        compute_dots();

        Eigen::Vector3d xy_center = (points[0] + points[2]) / 2;
        double alt_center = (points[0](2) + points[5](2)) / 2;
        center_ << xy_center(0), xy_center(1), alt_center;
    }

    void set_visual(int R, int G, int B, double opacity) {
        // Generate the shape
        if (points_.size() != 8) {
            cout << "Invalid number of cube points: " << points_.size() << endl;
            return;
        }

        const int num_faces = 6;
        const int vert_per_face = 4;
        int vert_lookup[num_faces][vert_per_face] = {
            {0, 1, 2, 3},
            {0, 1, 5, 4},
            {0, 3, 7, 4},
            {6, 5, 4, 7},
            {6, 2, 1, 5},
            {6, 7, 3, 2},
        };

        for (int f = 0; f < num_faces; f++) {
            sc::ShapePtr shape(new sp::Shape);
            shape->set_type(sp::Shape::Polygon);
            shape->set_opacity(opacity);
            sc::set(shape->mutable_color(), R, G, B);
            shape->set_persistent(true);
            for (int r = 0; r < vert_per_face; r++) {
                sp::Vector3d * p = shape->add_point();
                sc::set(p, points_[vert_lookup[f][r]]);
            }
            shapes_.push_back(shape);
        }
    }

 protected:
    std::vector<Eigen::Vector3d> points_;
    Eigen::Vector3d u;
    Eigen::Vector3d v;
    Eigen::Vector3d w;
    double u_dot_P0 = 0;
    double u_dot_P1 = 0;
    double v_dot_P0 = 0;
    double v_dot_P3 = 0;
    double w_dot_P0 = 0;
    double w_dot_P4 = 0;
};
} // namespace interaction
} // namespace scrimmage
#endif // INCLUDE_SCRIMMAGE_PLUGINS_INTERACTION_BOUNDARY_CUBOID_H_
