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

#include <scrimmage/plugin_manager/RegisterPlugin.h>
#include <scrimmage/plugins/controller/Unicycle3DControllerDirect/Unicycle3DControllerDirect.h>

REGISTER_PLUGIN(scrimmage::Controller, scrimmage::controller::Unicycle3DControllerDirect, Unicycle3DControllerDirect_plugin)

namespace scrimmage {
namespace controller {

void Unicycle3DControllerDirect::init(std::map<std::string, std::string> &params) {
    velocity_idx_ = vars_.declare("velocity", VariableIO::Direction::Out);
    turn_rate_idx_ = vars_.declare("turn_rate", VariableIO::Direction::Out);
    pitch_rate_idx_ = vars_.declare("pitch_rate", VariableIO::Direction::Out);
}

bool Unicycle3DControllerDirect::step(double t, double dt) {
    vars_.output(velocity_idx_, desired_state_->vel()(0));
    vars_.output(turn_rate_idx_, desired_state_->vel()(1));
    vars_.output(pitch_rate_idx_, desired_state_->vel()(2));
    return true;
}
} // namespace controller
} // namespace scrimmage
