#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

 
    if (!client.call(srv)){
         ROS_ERROR("Failed to call service safe_move");}
  
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    int div = img.step / 3;
    int white_loc = 0; // 0 == not found   1==left   2 == center  3 == right
//   ROS_WARN_STREAM("image h " << img.height << "image w " << img.width << "image s " << img.step);


    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
 
      for(int i = 0; i < img.height; i++) {
             for(int j = 0; j < img.step; j = j + 3) {
                  if(img.data[(i * img.step) + j] == white_pixel && img.data[(i * img.step) + j + 1] == white_pixel && img.data[(i * img.step) + j + 2] == white_pixel){
                                                          
                       if( j < div){
                           white_loc = 1;
                           break;}

                       else if( j  > 2 * div){
                           white_loc = 3;
                           break;}

                       else if( j  > div && j  < 2 * div){
                           white_loc = 2;
                           break;}  
     
                  } 
                 

                }
        }

/*    for(int i = 0; i < img.step; i++) {     
          for(int j = 0; j < img.height; j++) {      
                if(img.data[((j) * img.step) + i] == white_pixel){
 //                            ROS_WARN_STREAM(" i " << i << " j " << j);}
                            
                       if( i < div){
                           white_loc = 1;
                           break;}

                       else if( i > 2 * div){
                           white_loc = 3;
                           break;}

                       else if( i > div && i < 2 * div){
                           white_loc = 2;
                           break;}
                 }                      
        }
      }
*/

   if(white_loc == 1){
         drive_robot( 0.0 , 0.5);} 

    else if(white_loc == 2){
         drive_robot( 0.5 , 0.0);} 

    else if(white_loc == 3){
         drive_robot( 0.0 , -0.5);}

    else{
        drive_robot( 0.0 , 0.0);}  

}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
