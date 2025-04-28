#include <sl/Camera.hpp>
#include <chrono>
#include <cmath>
using namespace sl;
using namespace std;

//点云处理、颜色聚类实现物体和平面分割，将点云团分割成簇，保存。对基本图元的检测，对杂乱无章的点云对象做判断。

int main(int argc, char **argv) {

    //Create a ZED camera object
    Camera zed;

    // Set configuration parameters
    InitParameters init_params;
    init_params.camera_resolution = RESOLUTION::HD720;
    init_params.camera_fps = 60;
    init_params.depth_mode = DEPTH_MODE::ULTRA;
    init_params.coordinate_units = UNIT::MILLIMETER;
    
    //Open the camera 
    auto returned_state = zed.open(init_params);
    if(returned_state != ERROR_CODE::SUCCESS) {
        std::cout << "Error " << returned_state << ", exit program.\n";
        return EXIT_FAILURE;
    }
    // Get camera information (ZED serial number)
    auto camera_info = zed.getCameraInformation();
    printf("Hello! This is my serial number: %d\n", camera_info.serial_number);


    // Capture 50 frames and stop
    int i=0;
    sl::Mat image, depth, point_cloud;

    auto start = chrono::high_resolution_clock::now();
    while (i<100)
    {
        // Grab an image
        returned_state = zed.grab();
        // A new image is available id grab() returns  ERROR_CODE::SUCCESS
        if (returned_state == ERROR_CODE::SUCCESS)
        {
            // Get the left image
            zed.retrieveImage(image, VIEW::LEFT, MEM::GPU);
            // Get depth map. Depth map is aligned on the left iamge 
            //zed.retrieveMeasure(depth, MEASURE::DEPTH,MEM::GPU);
            // Get colored point cloud is aligned on the left image
            zed.retrieveMeasure(point_cloud, MEASURE::XYZBGRA);
            
            // Get and print distance value in mm at the center of the image 
            // We measure the distace camera - object using Euclidean distance 
            int x = image.getWidth()/2;
            int y = image.getHeight()/2;
            sl::float4 point_cloud_value;
            point_cloud.getValue(x, y, &point_cloud_value);
            
            //判断深度z是否是有限值，避免无穷大
            if(std::isfinite(point_cloud_value.z)) {
                float distance = sqrt(pow(point_cloud_value.x,2) + pow(point_cloud_value.y,2) + pow(point_cloud_value.z,2));
                cout << "distance to Camera at {" <<x<< ";" <<y<< "}: " <<distance<< "mm" <<endl;
            }else
                cout <<"The Distance can not be computed at {" <<x<< ";" <<y<< "}" <<endl;

            // Display the image resolution and its acquistion timestamp
            //cout << "Image resolution: " << image.getWidth() << "x" <<image.getHeight() 
            //     << " || Image timestamp " << image.timestamp.data_ns << endl; 
            i++;
        }
        
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop-start).count();
    cout << "Total time: " << duration << "ms || Frame per seconds: " << duration/i << "ms\n";
    
    //Close the camera 
    zed.close();
    return EXIT_SUCCESS;
}