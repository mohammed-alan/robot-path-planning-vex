#include "robot_path_planner.h"
#include <iostream>
#include <string>

// Simulated VEX robot hardware interface
namespace vex {
    // Motor class
    class motor {
    public:
        void setVelocity(double velocity, const std::string& unit) {
            std::cout << "Motor velocity set to " << velocity << " " << unit << std::endl;
        }
    };
    
    // Inertial Measurement Unit (IMU) class
    class inertial {
    public:
        void calibrate() {
            std::cout << "IMU calibration started" << std::endl;
            calibrating = true;
        }
        
        bool isCalibrating() {
            if (calibrating) {
                calibrating = false;
                return true;
            }
            return false;
        }
        
        double angle(const std::string& unit) {
            return 180.0; // Simulated angle
        }
        
    private:
        bool calibrating = false;
    };
    
    // Drivetrain class
    class drivetrain {
    public:
        void turn(const std::string& direction) {
            std::cout << "Drivetrain turning " << direction << std::endl;
        }
        
        void stop(const std::string& mode) {
            std::cout << "Drivetrain stopped with " << mode << " mode" << std::endl;
        }
    };
    
    // Constants
    const std::string rpm = "rpm";
    const std::string degrees = "degrees";
    const std::string seconds = "seconds";
    const std::string msec = "msec";
    const std::string right = "right";
    const std::string brake = "brake";
}

// Global VEX robot objects
vex::motor left_motor;
vex::motor right_motor;
vex::inertial imu;
vex::drivetrain Drivetrain;

// Wait function to simulate time delays
void wait(double time, const std::string& unit) {
    std::cout << "Waiting for " << time << " " << unit << std::endl;
}

// Calibrate inertial function as provided in the appendix
void cali_inertial() {
    left_motor.setVelocity(15, vex::rpm);
    right_motor.setVelocity(15, vex::rpm);
    
    imu.calibrate();
    while (imu.isCalibrating()) {}
    wait(1, vex::seconds);
    Drivetrain.turn(vex::right);
    wait(400, vex::msec);
    
    while (imu.angle(vex::degrees) < 166.0) {}
    Drivetrain.stop(vex::brake);
    wait(1, vex::seconds);
}

// Main function
int main() {
    std::cout << "Robot Path Planning Lab - Implementation" << std::endl;
    std::cout << "=======================================" << std::endl;
    
    // Create obstacles for testing
    // These coordinates match the red squares in the map description
    // You would replace these with actual obstacle detection in a real robot
    const int NUM_OBSTACLES = 4;
    int obstacleX[NUM_OBSTACLES] = {5, 10, 15, 10};
    int obstacleY[NUM_OBSTACLES] = {5, 10, 5, 15};
    
    // Initialize the robot path planner
    // Starting at (0,0), destination at (18,18), map size 20x20
    RobotPathPlanner pathPlanner(0, 0, 18, 18, 20, 20);
    
    // Add obstacles to the map
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        pathPlanner.markObstacle(obstacleX[i], obstacleY[i]);
        std::cout << "Obstacle added at (" << obstacleX[i] << ", " << obstacleY[i] << ")" << std::endl;
    }
    
    // Initialize the robot
    pathPlanner.initialize();
    
    // Execute the path planning algorithm
    pathPlanner.executePlanningAlgorithm();
    
    // Print the final path
    std::cout << "\nFinal path:" << std::endl;
    pathPlanner.getPath().print();
    
    return 0;
}
