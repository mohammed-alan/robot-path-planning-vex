#include "robot_path_planner.h"
#include <iostream>
#include <cmath>

// Constructor
RobotPathPlanner::RobotPathPlanner(int startX, int startY, int destX, int destY, int width, int height) :
    currentX(startX), currentY(startY), currentDirection(NORTH),
    finalX(destX), finalY(destY), mapWidth(width), mapHeight(height) {
    
    // Initialize obstacles map
    obstacles = new bool*[mapWidth];
    for (int i = 0; i < mapWidth; i++) {
        obstacles[i] = new bool[mapHeight];
        for (int j = 0; j < mapHeight; j++) {
            obstacles[i][j] = false;
        }
    }
    
    // Initialize path with maximum capacity of 10
    path = DoublyLinkedList(10);
}

// Destructor
RobotPathPlanner::~RobotPathPlanner() {
    // Clean up obstacles map
    for (int i = 0; i < mapWidth; i++) {
        delete[] obstacles[i];
    }
    delete[] obstacles;
}

// Initialize the robot
void RobotPathPlanner::initialize() {
    // Add starting location to path
    path.insert(currentX, currentY, START_LOCATION);
    
    // Calibrate the IMU
    calibrateInertial();
    
    // Set initial direction to NORTH (180 degrees)
    currentDirection = NORTH;
    
    // Configure drivetrain speed to 10 RPM
    std::cout << "Setting drivetrain speed to 10 RPM" << std::endl;
    
    // Print initial state
    printState();
}

// Execute the path planning algorithm
void RobotPathPlanner::executePlanningAlgorithm() {
    // Continue until destination is reached
    while (!isDestinationReached()) {
        // Determine movement priority
        Direction movementDirection = determineMovementPriority();
        
        // Check if we need to turn
        if (currentDirection != movementDirection) {
            // Turn to the new direction
            turn(movementDirection);
            
            // Update path with turning node
            updatePath(TURNING_NODE);
            
            // Handle turning trigger
            Node* currentNode = path.getTail();
            Node* previousNode = path.findLatestNecessaryNode();
            if (previousNode != currentNode) {
                handleTurningTrigger(currentNode, previousNode);
            }
        }
        
        // Move in the current direction
        move();
        
        // Check if an obstacle is detected
        if (isObstacleDetected(currentX, currentY)) {
            // Mark the obstacle
            markObstacle(currentX, currentY);
            
            // Update path with object detection node
            updatePath(OBJECT_DETECTION);
            
            // Handle turning trigger for obstacle avoidance
            Node* currentNode = path.getTail();
            Node* previousNode = path.findLatestNecessaryNode();
            if (previousNode != currentNode) {
                handleTurningTrigger(currentNode, previousNode);
            }
            
            // Determine new direction to avoid obstacle
            Direction newDirection = (currentDirection == NORTH) ? EAST : NORTH;
            
            // Turn to the new direction
            turn(newDirection);
            
            // Move to a safe distance (3-4 units) away from the obstacle
            for (int i = 0; i < 3; i++) {
                move();
                updatePath(REGULAR);
            }
        } else {
            // Update path with regular node
            updatePath(REGULAR);
        }
        
        // Check if path capacity is reached
        if (path.isFull()) {
            handleCapacityTrigger();
        }
        
        // Print current state
        printState();
    }
    
    std::cout << "Destination reached! Path planning completed successfully." << std::endl;
}

// Calibrate the inertial measurement unit (IMU)
void RobotPathPlanner::calibrateInertial() {
    std::cout << "Calibrating IMU..." << std::endl;
    
    // This is a simulation of the cali_inertial() function provided in the appendix
    // In a real implementation, this would call the actual hardware functions
    
    std::cout << "IMU calibration completed." << std::endl;
}

// Check if an obstacle is detected
bool RobotPathPlanner::isObstacleDetected(int x, int y) {
    // Check if the position is within map bounds
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
        return true;  // Treat out-of-bounds as obstacles
    }
    
    // Check if there's an obstacle at the position
    return obstacles[x][y];
}

// Mark an obstacle at the specified position
void RobotPathPlanner::markObstacle(int x, int y) {
    // Check if the position is within map bounds
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
        obstacles[x][y] = true;
    }
}

// Determine movement priority based on distance to destination
Direction RobotPathPlanner::determineMovementPriority() {
    // Calculate distances to destination in x and y directions
    int distX = finalX - currentX;
    int distY = finalY - currentY;
    
    // If both distances are equal, prioritize x direction (EAST)
    if (std::abs(distX) == std::abs(distY)) {
        return EAST;
    }
    
    // Otherwise, prioritize the direction with the greater distance
    return (std::abs(distX) > std::abs(distY)) ? EAST : NORTH;
}

// Turn the robot to a new direction
void RobotPathPlanner::turn(Direction newDirection) {
    std::cout << "Turning from " << (currentDirection == NORTH ? "NORTH" : "EAST") 
              << " to " << (newDirection == NORTH ? "NORTH" : "EAST") << std::endl;
    
    // In a real implementation, this would control the robot's motors to turn
    // and use the IMU to correct the angle as specified in step 9
    
    // Update the current direction
    currentDirection = newDirection;
}

// Move the robot in the current direction
void RobotPathPlanner::move() {
    // Move 1 unit (2 cm) in the current direction
    if (currentDirection == NORTH) {
        currentY++;
    } else {  // EAST
        currentX++;
    }
    
    std::cout << "Moved to position (" << currentX << ", " << currentY << ")" << std::endl;
}

// Update the path with a new node
void RobotPathPlanner::updatePath(NodeType nodeType) {
    // Add a new node to the path
    path.insert(currentX, currentY, nodeType);
    
    std::cout << "Added " << (nodeType == REGULAR ? "REGULAR" : 
                             nodeType == START_LOCATION ? "START_LOCATION" : 
                             nodeType == TURNING_NODE ? "TURNING_NODE" : "OBJECT_DETECTION") 
              << " node at (" << currentX << ", " << currentY << ")" << std::endl;
}

// Handle capacity trigger
void RobotPathPlanner::handleCapacityTrigger() {
    std::cout << "Path capacity reached. Removing regular nodes..." << std::endl;
    
    // Remove regular nodes between current position and latest necessary node
    path.removeRegularNodes();
    
    std::cout << "Regular nodes removed. Current path:" << std::endl;
    path.print();
}

// Handle turning trigger
void RobotPathPlanner::handleTurningTrigger(Node* currentNode, Node* previousNode) {
    std::cout << "Turning triggered. Removing regular nodes between necessary nodes..." << std::endl;
    
    // Remove regular nodes between current necessary node and previous necessary node
    path.removeRegularNodesBetweenNecessary(currentNode, previousNode);
    
    std::cout << "Regular nodes removed. Current path:" << std::endl;
    path.print();
}

// Get the current path
DoublyLinkedList& RobotPathPlanner::getPath() {
    return path;
}

// Print the current state
void RobotPathPlanner::printState() {
    std::cout << "Current position: (" << currentX << ", " << currentY << ")" << std::endl;
    std::cout << "Current direction: " << (currentDirection == NORTH ? "NORTH" : "EAST") << std::endl;
    std::cout << "Path size: " << path.getSize() << "/" << path.getCapacity() << std::endl;
}

// Check if destination is reached
bool RobotPathPlanner::isDestinationReached() {
    return (currentX == finalX && currentY == finalY);
}
