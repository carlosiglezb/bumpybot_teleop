//
// Created by Carlos on 7/10/22.
//

#ifndef SRC_WHEEL_CONTROL_PAD_H
#define SRC_WHEEL_CONTROL_PAD_H

#include <stdio.h>
#include <QPainter>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <sensor_msgs/JointState.h>
#include <std_msgs/Float64.h>
#include <QDebug>
#include <pluginlib/class_list_macros.h>

#include <ros/ros.h>
#include <ros/console.h>
#include <rviz/panel.h>

class QLineEdit;

namespace bumpybot_wheel_teleop
{


class WheelControlPad: public rviz::Panel
{
    Q_OBJECT
public:
    WheelControlPad( QWidget* parent = 0 );

    virtual void load( const rviz::Config& config );
    virtual void save( rviz::Config config ) const;

public Q_SLOTS:
            void setTopic( const QString& topic );

protected Q_SLOTS:
            void sendVel();
    void updateTopic();

protected:
    QDoubleSpinBox* wheel0_editor_;
    QLineEdit* output_topic_editor_;
    QString output_topic_;

    ros::Publisher velocity_publisher_;
    ros::NodeHandle nh_;

    float wheel0_current_;
};

} // end namespace bumpybot_wheel_teleop

#endif //SRC_WHEEL_CONTROL_PAD_H
