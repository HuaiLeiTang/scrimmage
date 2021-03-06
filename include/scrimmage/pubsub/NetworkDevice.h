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

#ifndef INCLUDE_SCRIMMAGE_PUBSUB_NETWORKDEVICE_H_
#define INCLUDE_SCRIMMAGE_PUBSUB_NETWORKDEVICE_H_

#include <scrimmage/plugin_manager/Plugin.h>
#include <scrimmage/pubsub/MessageBase.h>
#include <scrimmage/pubsub/Message.h>

#include <type_traits>
#include <list>
#include <memory>
#include <string>
#include <mutex> // NOLINT

namespace scrimmage {

class NetworkDevice {
 public:
    NetworkDevice() = default;
    NetworkDevice(NetworkDevice &rhs);
    NetworkDevice(NetworkDevice &&rhs);

    std::string get_topic() const;
    void set_topic(std::string topic);

    void set_msg_list(std::list<MessageBasePtr> msg_list);
    void clear_msg_list();

    void set_max_queue_size(unsigned int size);
    unsigned int max_queue_size();

    PluginPtr &plugin();

    void add_msg(MessageBasePtr msg);

    template <class T = MessageBase,
              class = std::enable_if_t<std::is_same<T, MessageBase>::value, void>>
    std::list<MessageBasePtr> msgs(bool pop_msgs = true, bool exclude_self = true) {
        mutex_.lock();
        std::list<MessageBasePtr> msg_list_cast;

        int id = plugin_->get_network_id();
        auto it = msg_list_.begin();
        while (it != msg_list_.end()) {

            if (!exclude_self || (*it)->sender != id) {
                msg_list_cast.push_back(*it);
            }

            it = pop_msgs ? msg_list_.erase(it) : std::next(it);
        }
        mutex_.unlock();
        return msg_list_cast;
    }

    template <class T,
              class = std::enable_if_t<!std::is_same<T, MessageBase>::value &&
                                       std::is_base_of<MessageBase, T>::value, void>>
    std::list<std::shared_ptr<T>> msgs(bool pop_msgs = true, bool exclude_self = true) {
        mutex_.lock();
        std::list<std::shared_ptr<T>> msg_list_cast;

        int id = plugin_->get_network_id();
        auto it = msg_list_.begin();
        while (it != msg_list_.end()) {

            if (!exclude_self || (*it)->sender != id) {
                auto msg_cast = std::dynamic_pointer_cast<T>(*it);
                if (msg_cast) {
                    msg_list_cast.push_back(msg_cast);
                } else {
                    print_str(std::string("WARNING: could not cast message on topic \"")
                        + topic_);
                }
            }

            it = pop_msgs ? msg_list_.erase(it) : std::next(it);
        }
        mutex_.unlock();

        return msg_list_cast;
    }

    template <class T,
              class = std::enable_if_t<!std::is_same<T, MessageBase>::value &&
                                       !std::is_base_of<MessageBase, T>::value, void>>
    std::list<std::shared_ptr<Message<T>>> msgs(bool pop_msgs = true, bool exclude_self = true) {
        return msgs<Message<T>>(pop_msgs, exclude_self);
    }

 protected:
    void print_str(std::string msg);
    std::string topic_;
    std::list<MessageBasePtr> msg_list_;
    PluginPtr plugin_ = std::make_shared<Plugin>();
    unsigned int max_queue_size_ = 1;
    std::mutex mutex_;
};

} // namespace scrimmage
#endif // INCLUDE_SCRIMMAGE_PUBSUB_NETWORKDEVICE_H_
