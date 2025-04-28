#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>


#include<opencv2/core/core.hpp>
#include<System.h>
#include<Mesh.h>
#include<MapPlane.h>


using namespace std;

class ImageGrabber
{
public:
    ImageGrabber(Planar_SLAM::System* pSLAM):mpSLAM(pSLAM){}

    void GrabRGBD(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD);

    Planar_SLAM::System* mpSLAM;
};


int main(int argc, char **argv)
{
    ros::init(argc,argv,"Planar_Slam");
    ros::start();

    if(argc != 3)
    {
        cerr << endl << "Usage:rosrun Planar_SLAM path_to_vocabulary path_to_settings" << endl;
        ros::shutdown();
        return 1;
    }

    ros::NodeHandle nh;

    ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Info);
    
    // Create SLam system
    Planar_SLAM::System SLAM(argv[1], argv[2], Planar_SLAM::System::RGBD, true);
    Planar_SLAM::Config::SetParameterFile(argv[2]);
    ImageGrabber igb(&SLAM);

    //Check settings file
    const string &strSettingsFile = argv[2];
    cv::FileStorage fsSettings(strSettingsFile, cv::FileStorage::READ);
    if(!fsSettings.isOpened())
    {
       cerr << "Failed to open settings file at: " << strSettingsFile << endl;
       exit(-1);
    }
    string image_topic = fsSettings["RosTopic.rgb"];
    string depth_topic = fsSettings["RosTopic.depth"];
    cout << image_topic << endl;
    cout << depth_topic << endl;
    fsSettings.release();


    message_filters::Subscriber<sensor_msgs::Image> sub_image(nh, image_topic, 1);
    /// /zed2i/zed_node/left/image_rect_color  /canglong2/image_raw
    message_filters::Subscriber<sensor_msgs::Image> sub_depth(nh, depth_topic, 1); 
    // /camera/depth/image_rect  /zed2i/zed_node/depth/depth_registered

    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> sync_pol;
    message_filters::Synchronizer<sync_pol> sync(sync_pol(10), sub_image,sub_depth);

    sync.registerCallback(boost::bind(&ImageGrabber::GrabRGBD,&igb,_1,_2));

    ros::spin();
    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory
    SLAM.SaveTrajectoryTUM("CameraTrajectory.txt");
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");
    SLAM.SaveMesh("MeshMap.ply");

    ros::shutdown();

    return 0;

}
void ImageGrabber::GrabRGBD(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD)
{
    // Copy the ros image message to cv::Mat.
    cv_bridge::CvImageConstPtr cv_ptrRGB;
    try
    {
        
        cv_ptrRGB = cv_bridge::toCvShare(msgRGB);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv_bridge::CvImageConstPtr cv_ptrD;
    try
    {
        cv_ptrD = cv_bridge::toCvShare(msgD);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    mpSLAM->TrackRGBD(cv_ptrRGB->image,cv_ptrD->image,cv_ptrRGB->header.stamp.toSec());
}

