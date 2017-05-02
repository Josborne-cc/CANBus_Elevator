The creation of an elevator with CAN-Bus and ethernet communication

Phase 1: 

    This phase involved the design of a CAN Bus system for transfering data between each node

    The nodes created were:
    
    - Controller Node
    - Elevator Car Node
    - 3 Floor Nodes
    - PID Dummy node(to simulate the car movement before the elevator was built).
    
    Each node uses an Axman development board with a core HCS12 processor.
    
    Notes on compilation for phase 1:
    
    - Code warrior ver. 5.1 was used.
    - Each node needs to be created as a separate project and the common libraries must be included in each of the respective projects. 
    
 

Phase 2:

    This phase involved the addition of a server so that the elevator could be access from a remote PC. In are case we only needed to access the CAN system from the local network.  Two new programs were created.  A server program for accessing the CAN system from a seperate computer and a communication program for grabbing the data from the CAN network and putting it into a database for access by the server.  A named pipe was also used for controlling the elevator over the network.  

    This folder contains three subfolders each of which is a different componet of the project.
    CAN Network:
        This is an update of the phase one to complete compatibility with the new phase 2 requirements.
    Server: 
        This is the program for accessing the system remotely it was done mostly in PHP and HTML.  This program was written by Loren Copeland.  
        To use this program the elevator folder must be placed under /var/www/html and the group and passwords file must be placed under /var/www
    Server Comm Program:
        This is for transfering the data from the CAN network to a MySQL database and for recieving controls from the remote PC
    
    Notes on compilation for phase 2:
    
    The server and the server comm program are both run on a linux pc.  I used (ubuntu 16.04).  
    The following must be installed on the computer:
        - boost libraries
        - apache2
        - MySQL
        - PHP with MySQL and LAMP extentions
    g++ was used to compile the server comm program

Phase 3:

    - This phase involved the simulation of the motor control calculations in MatLab it was completed by another group member

Phase 4:

    This phase involved taking the MatLab simulation and converting it to a C program to be run on the system and then combining all the other parts together into a complete system. 
   
    


