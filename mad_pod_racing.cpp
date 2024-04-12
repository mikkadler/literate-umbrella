#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <corecrt_math_defines.h>

using namespace std;

class Pod {
public:
    string name; //name to identfy pod
    int x; // x position of your pod
    int y; // y position of your pod
    int vx; // x speed of your pod
    int vy; // y speed of your pod
    int angle; // angle of your pod
    int next_checkpoint_id; // next checkpoint id of your pod

    string action_string; // thrust or boost or shield
    int move_target_x; // x position to face and move the pod
    int move_target_y; // y position to face and move the pod

    double next_checkpoint_dist;
    int turning_cooldown; // number representing turns to perform 0 thrust turning for each approach

    Pod(string p_name) {
        name = p_name;
        x = 0;
        y = 0;
        vx = 0;
        vy = 0;
        angle = 0;
        next_checkpoint_id = 0;
        action_string = "100";
        move_target_x = 0;
        move_target_y = 0;
        turning_cooldown = 1;
    };
    void calculate_thrust(vector<vector<int>> checkpoints, int tick);
    void set_move_target(vector<vector<int>> checkpoints, Pod opp1);
};

void Pod::calculate_thrust(vector<vector<int>> checkpoints, int tick) {
    action_string = "100";

    const int angle_to_start_throttle = 120;
    const double time_to_no_throttle = 0;

    int next_check_x = checkpoints[next_checkpoint_id][0];
    int next_check_y = checkpoints[next_checkpoint_id][1];
    int current_index;
    current_index = next_checkpoint_id - 1;
    if (current_index < 0) {
        current_index = checkpoints.size() - 1;
    }

    int current_check_x = checkpoints[current_index][0];
    int current_check_y = checkpoints[current_index][1];

    int distance_delta_x = next_check_x - x;
    int distance_delta_y = next_check_y - y;

    next_checkpoint_dist = sqrt(pow(distance_delta_x, 2) + pow(distance_delta_y, 2));

    int waypoint_distance_delta_x = next_check_x - current_check_x;
    int waypoint_distance_delta_y = next_check_y - current_check_y;

    double movement_angle = atan2(vy, vx) * 180 / M_PI;
    if (movement_angle < 0) {
        movement_angle += 360; // no negative angles
    }
    double waypoint_angle_for_boost = atan2(waypoint_distance_delta_y, waypoint_distance_delta_x) * 180 / M_PI;
    if (waypoint_angle_for_boost < 0) {
        waypoint_angle_for_boost += 360; // no negative angles
    }
    double waypoint_angle = atan2(distance_delta_y, distance_delta_x) * 180 / M_PI;
    if (waypoint_angle < 0) {
        waypoint_angle += 360; // no negative angles
    }
    
    double relative_waypoint_angle = angle - waypoint_angle;

    cerr << "pod angle:" << angle << " waypoint angle:" << waypoint_angle << std::endl;
    cerr << "move angle:" << movement_angle << " relative waypoint angle:" << relative_waypoint_angle << std::endl;

    double delta_angle = waypoint_angle_for_boost - movement_angle;
    if (delta_angle < 0) {
        delta_angle *= -1;
    }

    cerr << "delta angle for boost:" << delta_angle << std::endl;

    double speed = sqrt(pow(vx, 2) + pow(vy, 2));

    //if (next_checkpoint_dist > 5000 && delta_angle < 3 && speed > 0 ) {
    //    return "BOOST";
    //}
    if (name == "p1" && tick == 0) {
        action_string = "BOOST";
        return;
    }
    if (tick == 0) {
        action_string = "100";
        return;
    }

    if (speed * time_to_no_throttle > next_checkpoint_dist) {
        cerr << "player:" << name << " BREAKING:" << std::endl;
        action_string = "40";
        return;
    }
    if (relative_waypoint_angle > angle_to_start_throttle || relative_waypoint_angle < (-1 * angle_to_start_throttle)) {
        cerr << "player:" << name << " ADJUSTING:" << std::endl;
        action_string = "100";
        return;
    }
}

void Pod::set_move_target(vector<vector<int>> checkpoints, Pod opp1) {
    if (name == "p2") {
        move_target_x = opp1.x + opp1.vx * 3;
        move_target_y = opp1.y + opp1.vy * 3;
        return;
    }

    double LAMBDA = 3;

    move_target_x = checkpoints[next_checkpoint_id][0] - vx * LAMBDA;
    move_target_y = checkpoints[next_checkpoint_id][1] - vy * LAMBDA;

    double speed = sqrt(pow(vx, 2) + pow(vy, 2));

    cerr << "pods distance to next checkpoint:" << next_checkpoint_dist << " speed times x:" << speed * 4.8 << std::endl;

    // calculate angle for next turning point on track
    // use angle to set turning cooldown value

    if (speed * 4.8 > next_checkpoint_dist) {
        int next_index;
        next_index = next_checkpoint_id + 1;
        if (next_index > checkpoints.size() - 1) {
            next_index = 0;
        }
        move_target_x = checkpoints[next_index][0] - vx * LAMBDA;
        move_target_y = checkpoints[next_index][1] - vy * LAMBDA;
        if (turning_cooldown > 0) {
            action_string = "0";
            turning_cooldown--;
        }
    } else {
        turning_cooldown = 1;
    }
}

int main()
{
    int laps;
    cin >> laps; cin.ignore();
    int checkpoint_count;
    cin >> checkpoint_count; cin.ignore();
    vector<vector<int>> checkpoints;
    for (int i = 0; i < checkpoint_count; i++) {
        int checkpoint_x;
        int checkpoint_y;
        cin >> checkpoint_x >> checkpoint_y; cin.ignore();
        checkpoints.push_back(vector<int>{checkpoint_x, checkpoint_y});
    }

    int tick = 0;
    
    // create pod objects
    Pod player1 = Pod("p1");
    Pod player2 = Pod("p2");
    Pod opp1 = Pod("");
    Pod opp2 = Pod("");

    // game loop
    while (1) {
        cin >> player1.x >> player1.y >> player1.vx >> player1.vy >> player1.angle >> player1.next_checkpoint_id; cin.ignore();
        cin >> player2.x >> player2.y >> player2.vx >> player2.vy >> player2.angle >> player2.next_checkpoint_id; cin.ignore();
        cin >> opp1.x >> opp1.y >> opp1.vx >> opp1.vy >> opp1.angle >> opp1.next_checkpoint_id; cin.ignore();
        cin >> opp2.x >> opp2.y >> opp2.vx >> opp2.vy >> opp2.angle >> opp2.next_checkpoint_id; cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        player1.calculate_thrust(checkpoints, tick);
        player2.calculate_thrust(checkpoints, tick);        
    
        player1.set_move_target(checkpoints, opp1);
        player2.set_move_target(checkpoints, opp1);

        cout << player1.move_target_x << " " << player1.move_target_y << " " << player1.action_string << endl;
        cout << player2.move_target_x << " " << player2.move_target_y << " " << player2.action_string << endl;

        tick++;
    }

}