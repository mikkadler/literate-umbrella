#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <corecrt_math_defines.h>

using namespace std;

double getAngle(int delta_x, int delta_y);

int main()
{

int previous_tick_checkpoint_distance = -1;
int boost_left = 1;
const int angle_to_start_throttle = 120;
const double time_to_no_throttle = 4.5;
int prev_tick_x = -1;
int perv_tick_y = -1;

    // game loop
    while (1) {
        int x;
        int y;
        int next_checkpoint_x; // x position of the next check point
        int next_checkpoint_y; // y position of the next check point
        int next_checkpoint_dist; // distance to the next checkpoint
        int next_checkpoint_angle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> next_checkpoint_x >> next_checkpoint_y >> next_checkpoint_dist >> next_checkpoint_angle; cin.ignore();
        int opponent_x;
        int opponent_y;
        cin >> opponent_x >> opponent_y; cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        int thrust = 100;
        bool boost = false;
        bool shield = false;

        if (previous_tick_checkpoint_distance < 0) {
            // first tick
            previous_tick_checkpoint_distance = next_checkpoint_dist;
            prev_tick_x = x;
            perv_tick_y = y;
        }

        int speed = previous_tick_checkpoint_distance - next_checkpoint_dist;
        int speed_x = x - prev_tick_x;
        int speed_y = y - perv_tick_y;
        previous_tick_checkpoint_distance = next_checkpoint_dist;
        prev_tick_x = x;
        perv_tick_y = y;

        

        if (boost_left > 0 && next_checkpoint_dist > 5000 && (next_checkpoint_angle < 3 || next_checkpoint_angle > -3) && speed > 0 ) {
            boost = true;
            boost_left--;
        }        

        cerr << "Speed:" << speed << " dist to next:" << next_checkpoint_dist << std::endl;
        cerr << "Speed x:" << speed_x << " speed y:" << speed_y << std::endl;
/*
        double movement_angle = getAngle(speed_x, speed_y);
        int delta_w_x = next_checkpoint_x - x;
        int delta_w_y = next_checkpoint_y - y;
        double waypoint_angle = getAngle(delta_w_x, delta_w_y);
        cerr << "move angle:" << movement_angle << " next waypoint angle:" << waypoint_angle << std::endl;
*/
        if (speed * time_to_no_throttle > next_checkpoint_dist) {
            thrust = 40;
        }
        if (next_checkpoint_angle > angle_to_start_throttle || next_checkpoint_angle < (-1 * angle_to_start_throttle)) {
            thrust = 40;
        }

        next_checkpoint_x = next_checkpoint_x - speed_x * 1.5;
        next_checkpoint_y = next_checkpoint_y - speed_y * 1.5;


        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"
        if (boost) {
            cout << next_checkpoint_x << " " << next_checkpoint_y << " BOOST" << endl;
        } else if (shield) {
            cout << next_checkpoint_x << " " << next_checkpoint_y << " SHIELD" << endl;
        } else {
            cout << next_checkpoint_x << " " << next_checkpoint_y << " " << thrust << endl;
        }
    }

}

double getAngle(int delta_x, int delta_y) 
{
        return atan2(delta_y, delta_x) * 180 / M_PI;
};