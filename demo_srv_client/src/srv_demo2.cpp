//
// Created by ray on 5/5/22.
//
/*
    需求:
        编写两个节点实现服务通信，客户端节点需要提交两个整数到服务器
        服务器需要解析客户端提交的数据，相加后，将结果响应回客户端，
        客户端再解析

    服务器实现:
        1.包含头文件
        2.初始化 ROS 节点
        3.创建 ROS 句柄
        4.创建 服务 对象
        5.回调函数处理请求并产生响应
        6.由于请求有多个，需要调用 ros::spin()

*/
#include "ros/ros.h"
#include "demo_srv_client/ExecuteAddInts.h"
#include "roport/ExecuteJointPosition.h"

// bool 返回值由于标志是否处理成功
bool doReq(demo_srv_client::ExecuteAddInts::Request& req,
           demo_srv_client::ExecuteAddInts::Response& resp){
    int num1 = req.num1;
    int num2 = req.num2;

    ROS_INFO("服务器接收到的请求数据为:num1 = %d, num2 = %d",num1, num2);

    //逻辑处理
    if (num1 < 0 || num2 < 0)
    {
        ROS_ERROR("提交的数据异常:数据不可以为负数");
        return false;
    }

    //如果没有异常，那么相加并将结果赋值给 resp
    ros::Duration(5).sleep();
    resp.sum = num1 + num2;
    return true;
}

bool doReq1(roport::ExecuteJointPosition::Request& req,
            roport::ExecuteJointPosition::Response& resp){
    ROS_INFO("Start left CB!");
    ros::Duration(1).sleep();
    resp.result_status=resp.SUCCEEDED;
    ROS_INFO("Finish left CB!");
    return true;
}

bool doReq2(roport::ExecuteJointPosition::Request& req,
            roport::ExecuteJointPosition::Response& resp){
    ROS_INFO("Start right CB!");
    ROS_WARN("Received joint cmd: %.4f", req.goal_state.position[0]);
    ros::Duration(2).sleep();
    resp.result_status=resp.SUCCEEDED;
    ROS_INFO("Finish right CB!");
    return true;
}

bool doReq3(roport::ExecuteJointPosition::Request& req,
            roport::ExecuteJointPosition::Response& resp){
    ROS_INFO("Start torso CB!");
    ros::Duration(3).sleep();
    resp.result_status=resp.SUCCEEDED;
    ROS_INFO("Finish torso CB!");
    return true;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    // 2.初始化 ROS 节点
    ros::init(argc,argv,"Server_2");
    // 3.创建 ROS 句柄
    ros::NodeHandle nh;
    // 4.创建 服务 对象
    ros::ServiceServer server2 = nh.advertiseService("/panda_right/execute_joint_position_srv_control",doReq2);
    ROS_INFO("服务已经启动....");
    //     5.回调函数处理请求并产生响应
    //     6.由于请求有多个，需要调用 ros::spin()
    ros::spin();
    return 0;
}
