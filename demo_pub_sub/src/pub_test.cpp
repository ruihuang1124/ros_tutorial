/*
    需求: 实现基本的话题通信，一方发布数据，一方接收数据，
         实现的关键点:
         1.发送方
         2.接收方
         3.数据(此处为普通文本)

         PS: 二者需要设置相同的话题


    消息发布方:
        循环发布信息:HelloWorld 后缀数字编号

    实现流程:
        1.包含头文件 
        2.初始化 ROS 节点:命名(唯一)
        3.实例化 ROS 句柄
        4.实例化 发布者 对象
        5.组织被发布的数据，并编写逻辑发布数据

*/
// 1.包含头文件 
#include "ros/ros.h"
#include "std_msgs/String.h" //普通文本类型的消息
#include <sstream>
#include <sensor_msgs/JointState.h>

int main(int argc, char  *argv[])
{   
    //设置编码
    setlocale(LC_ALL,"");

    //2.初始化 ROS 节点:命名(唯一)
    // 参数1和参数2 后期为节点传值会使用
    // 参数3 是节点名称，是一个标识符，需要保证运行后，在 ROS 网络拓扑中唯一
    ros::init(argc,argv,"talker");
    //3.实例化 ROS 句柄
    ros::NodeHandle nh;//该类封装了 ROS 中的一些常用功能

    //4.实例化 发布者 对象
    //泛型: 发布的消息类型
    //参数1: 要发布到的话题
    //参数2: 队列中最大保存的消息数，超出此阀值时，先进的先销毁(时间早的先销毁)
    ros::Publisher pub = nh.advertise<std_msgs::String>("chatter",10);
    ros::Publisher pub_head_test = nh.advertise<sensor_msgs::JointState>("/curi_head/joint_states", 1);
    ros::Publisher pub_torso_test = nh.advertise<sensor_msgs::JointState>("/curi_torso/joint_states", 1);
    sensor_msgs::JointState head_msg, torso_msg;
    head_msg.position.resize(2);
    head_msg.position[0] = 0;
    head_msg.position[1] = 1;
    torso_msg.position.resize(3);
    //5.组织被发布的数据，并编写逻辑发布数据
    //数据(动态组织)
    std_msgs::String msg;
    // msg.data = "你好啊！！！";
    std::string msg_front = "Hello 你好！"; //消息前缀
    int count = 0; //消息计数器

    //逻辑(一秒10次)
    ros::Rate r(10);

    //节点不死
    while (ros::ok())
    {
        // head_msg.header.stamp.sec = ros::Time::now().toSec();
        // torso_msg.header.stamp.sec = ros::Time::now().toSec();
        head_msg.header.stamp = ros::Time::now();
        torso_msg.header.stamp = ros::Time::now();
        //使用 stringstream 拼接字符串与编号
        // std::stringstream ss;
        // ss << msg_front << count;
        // msg.data = ss.str();
        // //发布消息
        // pub.publish(msg);
        // //加入调试，打印发送的消息
        // ROS_INFO("发送的消息:%s",msg.data.c_str());

        // //根据前面制定的发送贫频率自动休眠 休眠时间 = 1/频率；
        // r.sleep();
        // count++;//循环结束前，让 count 自增
        //暂无应用
        pub_head_test.publish(head_msg);
        pub_torso_test.publish(torso_msg);
        r.sleep();
        ros::spinOnce();
    }


    return 0;
}
