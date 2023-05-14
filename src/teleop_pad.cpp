//
// Created by Carlos on 11/3/21.
//

#include "teleop_pad.h"

namespace bumpybot_teleop
{
    TeleopPanel::TeleopPanel(QWidget *parent)
            : rviz::Panel(parent), linear_velocity_x_(0), linear_velocity_y_(0), angular_velocity_(0) {

        //TODO get topic name from config file
        QVBoxLayout *topic_layout = new QVBoxLayout;
        topic_layout->addWidget(new QLabel("Teleop Topic:"));
        output_topic_editor_ = new QLineEdit;
        topic_layout->addWidget(output_topic_editor_);

        topic_layout->addWidget(new QLabel("Linear Velocity (x):"));
        lin_vel_editor_x_ = new QDoubleSpinBox;
        lin_vel_editor_x_->setRange(-5.0,5.0);
        lin_vel_editor_x_->setSingleStep(0.01);
        lin_vel_editor_x_->setValue(0.0);
        topic_layout->addWidget(lin_vel_editor_x_);
//        spinbox->setProperty("param_widget_type", kParamDouble);
//        widgets_[name] = spinbox;
//        AddLabeledRow(name, spinbox);


        topic_layout->addWidget(new QLabel("Linear Velocity (y):"));
        lin_vel_editor_y_ = new QDoubleSpinBox;
        lin_vel_editor_y_->setRange(-5.0,5.0);
        lin_vel_editor_y_->setSingleStep(0.01);
        lin_vel_editor_y_->setValue(0.0);
        topic_layout->addWidget(lin_vel_editor_y_);

        topic_layout->addWidget(new QLabel("Angular Velocity (z):"));
        ang_vel_editor_ = new QDoubleSpinBox;
        ang_vel_editor_->setRange(-5.0,5.0);
        ang_vel_editor_->setSingleStep(0.01);
        ang_vel_editor_->setValue(0.0);
        topic_layout->addWidget(ang_vel_editor_);

        QHBoxLayout *layout = new QHBoxLayout;
        layout->addLayout(topic_layout);
        setLayout(layout);

        QTimer *output_timer = new QTimer(this);

        // set values to get updated from GUI
        connect(output_topic_editor_, SIGNAL(editingFinished()), this, SLOT(updateTopic()));
        connect(lin_vel_editor_x_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [=](double d){ linear_velocity_x_ = d; });
        connect(lin_vel_editor_y_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [=](double d){ linear_velocity_y_ = d; });
        connect(ang_vel_editor_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [=](double d){ angular_velocity_ = d; });

        // set the timer to call update() at specified rate
        connect(output_timer, SIGNAL(timeout()), this, SLOT(sendVel()));
        output_timer->start(100);   // in milliseconds
    }

    void TeleopPanel::updateTopic()
    {
        setTopic( output_topic_editor_->text() );
    }

    // Set the topic name we are publishing to.
    void TeleopPanel::setTopic( const QString& new_topic )
    {
        // Only take action if the name has changed.
        if( new_topic != output_topic_ )
        {
            output_topic_ = new_topic;
            // If the topic is the empty string, don't publish anything.
            if( output_topic_ == "" )
            {
                velocity_publisher_.shutdown();
            }
            else
            {

                velocity_publisher_ = nh_.advertise<geometry_msgs::Twist>( output_topic_.toStdString(), 1 );
            }

            Q_EMIT configChanged();
        }

        // TODO Gray out the control widget when the output topic is empty.
    }

    void TeleopPanel::sendVel()
    {
        if( ros::ok() && velocity_publisher_ )
        {
            geometry_msgs::Twist twist;
            twist.linear.x = linear_velocity_x_;
            twist.linear.y = linear_velocity_y_;
            twist.linear.z = 0;
            twist.angular.x = 0;
            twist.angular.y = 0;
            twist.angular.z = angular_velocity_;
            velocity_publisher_.publish( twist );
        }
    }

    void TeleopPanel::save( rviz::Config config ) const
    {
        rviz::Panel::save( config );
        config.mapSetValue( "Topic", output_topic_ );
    }

    // Load all configuration data for this panel from the given Config object.
    void TeleopPanel::load( const rviz::Config& config )
    {
        rviz::Panel::load( config );
        QString topic;
        if( config.mapGetString( "Topic", &topic ))
        {
            output_topic_editor_->setText( topic );
            updateTopic();
        }
    }

} //end namespace bumpybot_teleop

PLUGINLIB_EXPORT_CLASS(bumpybot_teleop::TeleopPanel,rviz::Panel)