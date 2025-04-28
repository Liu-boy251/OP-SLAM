# OP-SLAM
 A semantic SLAM for semi-structured environments using object and plane detection.Our envirnments: ubuntu20.04,ROS1. GPU: RTX3080.
# Installation
The dependent libraries include:
* opencv4.6
* DBoW2
* G2o
* PLC-1.10
* Pangolin
* ceres
* gtsam
# Building
After libraries are successfully installed, we can proceed with the compilaation:
          ./build.sh
# Run
For running the program:
1. cd OP-SLAM
2. ./Examples/ROS/OPSLAM/build/OP_ros ./Vocabulary/ORBvoc.txt ./Examples/RGB-D/zed2i.yaml

