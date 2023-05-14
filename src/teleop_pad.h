//
// Created by Carlos on 11/2/21.
//

#ifndef SRC_TELEOP_PAD_H
#define SRC_TELEOP_PAD_H

#include <stdio.h>
#include <QPainter>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <geometry_msgs/Twist.h>
#include <QDebug>
#include <pluginlib/class_list_macros.h>

#include <ros/ros.h>
#include <ros/console.h>
#include <rviz/panel.h>

class QLineEdit;

namespace bumpybot_teleop
{


class TeleopPanel: public rviz::Panel
{
    Q_OBJECT
    public:
        TeleopPanel( QWidget* parent = 0 );

        virtual void load( const rviz::Config& config );
        virtual void save( rviz::Config config ) const;

    public Q_SLOTS:
        void setTopic( const QString& topic );

    protected Q_SLOTS:
        void sendVel();
        void update_Linear_Velocity_x();
        void update_Linear_Velocity_y();
        void update_Angular_Velocity();
        void updateTopic();

    protected:
        QDoubleSpinBox* lin_vel_editor_x_;
        QDoubleSpinBox* lin_vel_editor_y_;
        QDoubleSpinBox* ang_vel_editor_;
        QLineEdit* output_topic_editor_;
        QString output_topic_;

        ros::Publisher velocity_publisher_;
        ros::NodeHandle nh_;

        float linear_velocity_x_;
        float linear_velocity_y_;
        float angular_velocity_;
    };
} //end namespace trikey_teleop

#endif //SRC_TELEOP_PAD_H
