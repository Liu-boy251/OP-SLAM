###运行数据集（在/save 文件夹内）
source /home/liu-420/tmp/vision_opencv/devel/setup.bash

./Examples/RGB-D/OP_SLAM ./Vocabulary/ORBvoc.txt ./Examples/RGB-D/ICL.yaml ./datasets/living_room_traj1 ./datasets/living_room_traj1/associations.txt

#(运行报错)
./op-slam-ros/build/OPSLAM ./Vocabulary/ORBvoc.txt ./Examples/RGB-D/zed2i.yaml 

#(可运行)
./Examples/ROS/OPSLAM/build/OP_ros ./Vocabulary/ORBvoc.txt ./Examples/RGB-D/zed2i.yaml 

#(可运行)2024.3.27
./Examples/ROS/OPSLAM/build/OP_ros ./Vocabulary/ORBvoc.txt ./Examples/RGB-D/cruzr.yaml 
 1894  rostopic list 
 1895  rqt_image_view 
 1896  python robot_keyboard_teleop.py
