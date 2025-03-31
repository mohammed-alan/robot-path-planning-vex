#ifndef ROBOT_PATH_PLANNER_H
#define ROBOT_PATH_PLANNER_H

#include "doubly_linked_list.h"

// Direction enumeration
enum Direction {
    NORTH = 180,  // 180 degrees (positive y direction)
    EAST = 270    // 270 degrees (positive x direction)
};

class RobotPathPlanner {
private:
    // Robot position and orientation
    int currentX;
    int currentY;
    Direction currentDirection;
    
    // Final destination
    const int finalX;
    const int finalY;
    
    // Map dimensions and obstacles
    const int mapWidth;
    const int mapHeight;
    bool** obstacles;  // 2D array to track obstacles
    
    // Path data structure
    DoublyLinkedList path;
    
    // Helper functions
    bool isObstacleDetected(int x, int y);
    Direction determineMovementPriority();
    void turn(Direction newDirection);
    void move();
    void updatePath(NodeType nodeType);
    void handleCapacityTrigger();
    void handleTurningTrigger(Node* currentNode, Node* previousNode);
    
public:
    // Mark an obstacle at the specified position
    void markObstacle(int x, int y);
    // Constructor
    RobotPathPlanner(int startX, int startY, int destX, int destY, int width, int height);
    
    // Destructor
    ~RobotPathPlanner();
    
    // Initialize the robot
    void initialize();
    
    // Execute the path planning algorithm
    void executePlanningAlgorithm();
    
    // Calibrate the inertial measurement unit (IMU)
    void calibrateInertial();
    
    // Get the current path
    DoublyLinkedList& getPath();
    
    // Print the current state
    void printState();
    
    // Check if destination is reached
    bool isDestinationReached();
};

#endif // ROBOT_PATH_PLANNER_H
