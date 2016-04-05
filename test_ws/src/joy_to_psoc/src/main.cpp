#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <math.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Joy.h"

uint32_t makePacket(uint8_t address, uint8_t cmd,uint8_t data)
{
    uint32_t rtn = 0x00000000;
    rtn+=address; //rtn = 0x00 00 00 11
    rtn = rtn<<8; //rtn = 0x00 00 11 00
    rtn |= cmd; //rtn = 0x00 00 11 aa
    rtn = rtn <<8; //rtn = 0x00 11 aa 00
    rtn |= data; //rtn = 0x00 11 aa bb
    rtn = rtn <<8; //rtn = 0x11 aa bb 00
    uint8_t chksm = (address+cmd+data) & 0x7F; //checksum is 7 bit masked addition of the other 3 bytes. 7f= 0111 1111
    rtn |= chksm; //rtn = 0x11 aa bb 76
    return rtn;
}
void stopMotors(tty_fd)
{
    //Send command 0 0 to all 5 motor controllers
    uint32_t serialcmd;
    for(i=128;i<133;i++)
    {
        serialcmd = makePacket(i,0,0);
        write(tty_fd,&serialcmd,4);
        serialcmd = makePacket(i,4,0);
        write(tty_fd,&serialcmd,4);
    }
}
void goForward(unsigned char intensity)
{
    
}
void goBackward(unsigned char intensity)
{
    
}
void turnClockwise(unsigned char intensity)
{
    
}
void turnCounterclockwise(unsigned char intensity)
{
    
}
#define CMD_STOP    0x00
#define CMD_FWD     0x01
#define CMD_REV     0x02
#define CMD_CW      0x04
#define CMD_CCW     0x08

uint16_t makeserialpacket(unsigned char cmd,unsigned char intensity)
{
    uint16_t rtn = 0x0000;
    if(cmd)
    {
        rtn |=cmd; //ex. cmd = 0x08, rtn =0x0000; rtn = 0x0008
        rtn = rtn << 8; //rtn = 0x0800
        rtn = rtn & 0x0f00;
    }
    rtn |= intensity; //ex. intensity = 0xff; rtn = 0x08FF
    rtn = (rtn <<4) & 0xfff0;//rtn is now 0x8FF0
    //Checksum is the bitwise addition of the 3 nibbles with the overflow discarded. (cmd, int high, int low)
    //in our example, this would be (0x8 + 0xf + 0xf) or (0b1000 + 0b1111 + 0b1111) = 0b0110 or 0x6
    unsigned char chksm = (((intensity & 0x0f)+((intensity >>4)&0x0f) + (cmd&0x0f))&0x0f);
    rtn = rtn | chksm; //rtn is 0x8FF6
    return rtn; 
}
int16_t axispos = 0; //Axis Position as of last joystick reading, rounded to be between [-255,255]
int16_t prevaxispos = 0; //used to prevent repeats.
bool btn = false; //Button press notification. Switches our state from turning to forward.
bool stop = false; //SHUT DOWN EVERYTHING
void joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    int16_t val = floor(joy->axes[1]*255);
    if(prevaxispos !=val)
        axispos=val;
    btn = (joy->buttons[0]==1);
    stop = (joy->buttons[1]==1);
}
int main(int argc,char** argv)
{
    ros::init(argc,argv, "joy_to_psoc");
    ros::NodeHandle n;
    ros::Publisher publisher = n.advertise<std_msgs::String>("serialstream",1000); //I doubt we're gonna get 1000 unread messages but hey, stranger things right?
    
    struct termios tio;
    struct termios stdio;
    int tty_fd;
    fd_set rdset;

    unsigned char c='D';

    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    tty_fd=open(argv[1], O_RDWR | O_NONBLOCK);      
    if(tty_fd<0)
    {
        perror("Serial Open failure");
        return EXIT_FAILURE;
    }
    ROS_INFO("Opened %s (fd %d)",argv[1],tty_fd);
    cfsetospeed(&tio,B9600);            // 9600 baud
    cfsetispeed(&tio,B9600);            // 9600 baud

    tcsetattr(tty_fd,TCSANOW,&tio);

    ros::Subscriber joy_sub = n.subscribe<sensor_msgs::Joy>("joy",0,joyCallback);
    //Loop
    while (ros::ok())
    {
        if (read(tty_fd,&c,1)>0)
        {
            std_msgs::String msg;
            msg.data.push_back(c);
            while((read(tty_fd,&c,1)>0))
                    msg.data.push_back(c); //Get the whole message before publishing.
            publisher.publish(msg);              // if new data is available on the serial port, print it out
        }
        if(axispos)
        {
            uint16_t serialcmd=0;
            if(btn)
            {
                if(axispos>0)
                    turnClockwise(axispos,tty_fd);
                else
                    turnCounterclockwise(axispos,tty_fd);
            }
            else
            {
                if(axispos>0)
                    goForward(axispos,tty_fd);
                else
                    goBackward(axispos,tty_fd);
            }
            ROS_INFO("Sending command %04x",serialcmd);
            prevaxispos=axispos;
            axispos=0;
        }
        if(stop)
        {
            uint16_t serialcmd = makeserialpacket(CMD_STOP,0xaa);
            write(tty_fd,&serialcmd,2);
            ROS_INFO("Sending command %04x",serialcmd);
            stop=false;
        }
        ros::spinOnce();
    }

    close(tty_fd);
}
