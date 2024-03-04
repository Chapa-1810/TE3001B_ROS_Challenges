# ROS INTERFACES
In this packages, different interfaces are included, messages and services requests/responses.

 - **Messages**: ROS custom messages is basically a way of determining the data that is going to be used in our implementations, they can be seen basically as an struct of C++ or Python. They can also include, other, more complex type of messages such as geometry_msgs.

 - **Service**: ROS custom services one of the most important features for ROS Clients and Servers, because with this custom interfaces, it is possible to determine the type of data that a client needs to have within it's request, and the type of data that the responses can have sended by the server. They can also include more than one data point, and can also be seen as structs.  

## Declaration
For ROS messages or services, its important to first create a ROS package in which the `/msg` folder is created:

```
cd ~/ros_ws/src
ros2 pkg create --build--type ament_cmake <package_name>
mkdir <package_name>/msg <package_name>/srv
```
Once the `/msg` or `/srv` folder is created, any custom message or service can be generated only by creating it's .msg or .srv file, however one key factor of msgs/srvs is the ***NAME***, which most follow a CamelCase format and more in order to work, that is given to the file. This name, depe will affect the way in which the msg or srv is called later on other files.


## C++
ROS messages/srvs are called both in the same way in a C++ node, they must be included as a header file in both cases, replacing the `.msg` or `.srv` for a `.hpp`. At the same time, the name of this included file will change a little bit from the original, for the example it will be used <file_name> The way to include them is by following this format:

```
#include "<ros_package>/msg/<file_name>.hpp"
```

Some key factors when getting the custom msg to a different package or file, it's that besides the name of the file being able to have upper cases within it, when called for, they are depreciated and converted all into lower case letters. At the same time, if the file name has more than one upper case in it, then the way to call it in a c++ node should be to add a lower tab before the letter. For example:


**Interfaces**

| *FILE NAME* | *TO INCLUDE* |
| --- | --- |
| EXAMPLE.msg | example.hpp |
| Example.srv | example.hpp |
| OtherExample.msg |other_example.hpp |
| otherExample | *NOT INCLUDED* |








## Python

## TODO
Implement ROS interfaces within the same package
