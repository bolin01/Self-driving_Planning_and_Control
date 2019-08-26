/*
 *
 */

#ifndef PERCEPTION_H
#define PERCEPTION_H

#include <ros/ros.h>
#include <vector>
#include <hkj_msgs/VehicleState.h>
#include <hkj_msgs/RoadConditionVector.h>
#include <hkj_msgs/RoadCondition.h>
#include <iostream>

namespace mpc_traj_follower {

struct Obstacle {
  public:
    float pos_x[4] = {0.0, 0.0, 0.0, 0.0};
    float pos_y[4] = {0.0, 0.0, 0.0, 0.0};
};

class Perception {
  public:
    Perception(ros::NodeHandle& nh);
    ~Perception();

    /** [Objective]
     * read offline roadmap info from CSV file
     * return true if roadmap info is correctly obtained
     * */
    bool readRoadmapFromCSV();
    /** [Objective]
     * prepare perception msg based on state feedback from plant_model_node
     * return true if perception msg is correctly prepared
     * */
    bool preparePerceptionMsg(double pos_x, double pos_y, double yaw_ang);
    void publishPerceptionMsg();

  private:
    /* ROS */
    ros::NodeHandle nh_;
    ros::Subscriber sub_cur_state_;             // receive from plant_model_node
    ros::Publisher  pub_road_cond_;             // publish to pnc_node

    /* Variable */
    // perception
    std::vector<std::vector<float>> waypoints_; // an offline map of road bound waypoints read from csv
    std::vector<Obstacle> obs_rects_;           // an offline map of obstacle rectangles read from csv
    std::vector<float> left_bound_wps_;         // left bound waypoints vector
    std::vector<float> right_bound_wps_;        // right bound waypoints vector
    std::vector<float> mid_line_wps_;           // middle line waypoints vector
    void parseRoadMap(const std::string& roadmap);
    void parseRoadMapLine(const std::string& line);
    // vehicle state
    bool new_veh_state_;                        // if a new vehicle state is received
    std::vector<float> received_veh_state_;     // vehicle state received from plant_model_node
    float state_pos_x_;                         // state: global position x  [m]
    float state_pos_y_;                         // state: global position y  [m]
    float state_yaw_ang_;                       // state: yaw andgle         [rad]
    // trajectory
    std::vector<std::vector<float>> states_traj_; 
    int traj_vt_size_;

    void vehicleStateCallback(const hkj_msgs::VehicleState::ConstPtr& msg);
    void clearVehicleState();
    void clearPerception();
};


} /* end of namespace mpc_traj_follower */


#endif /* PNC_H */