//
// Created by Carlos on 7/10/22.
//

#include "wheel_control_pad.h"

namespace bumpybot_wheel_teleop
{

WheelControlPad::WheelControlPad(QWidget *parent)
        : rviz::Panel(parent), wheel0_current_(0){

  QVBoxLayout *topic_layout = new QVBoxLayout;
  topic_layout->addWidget(new QLabel("Teleop Topic:"));
  output_topic_editor_ = new QLineEdit;
  topic_layout->addWidget(output_topic_editor_);

  topic_layout->addWidget(new QLabel("Wheel 0 Des Current (A):"));
  wheel0_editor_ = new QDoubleSpinBox;
  wheel0_editor_->setRange(-4.0,4.0);
  wheel0_editor_->setSingleStep(0.1);
  wheel0_editor_->setValue(0.0);
  topic_layout->addWidget(wheel0_editor_);
//        spinbox->setProperty("param_widget_type", kParamDouble);
//        widgets_[name] = spinbox;
//        AddLabeledRow(name, spinbox);

  QHBoxLayout *layout = new QHBoxLayout;
  layout->addLayout(topic_layout);
  setLayout(layout);

  QTimer *output_timer = new QTimer(this);

  // set values to get updated from GUI
  connect(output_topic_editor_, SIGNAL(editingFinished()), this, SLOT(updateTopic()));
  connect(wheel0_editor_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          [=](double d){ wheel0_current_ = d; });

  // set the timer to call update() at specified rate
  connect(output_timer, SIGNAL(timeout()), this, SLOT(sendVel()));
  output_timer->start(100);   // in milliseconds
}

void WheelControlPad::updateTopic()
{
  setTopic( output_topic_editor_->text() );
}

// Set the topic name we are publishing to.
void WheelControlPad::setTopic( const QString& new_topic )
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
      ROS_INFO("Topic name changed");
      velocity_publisher_ = nh_.advertise<std_msgs::Float64>( output_topic_.toStdString(), 1 );
    }

    Q_EMIT configChanged();
  }

  // TODO Gray out the control widget when the output topic is empty.
}

void WheelControlPad::sendVel()
{
  if( ros::ok() && velocity_publisher_ )
  {
    ROS_INFO("Entering sendVal");
//    sensor_msgs::JointState wheel0;
//    wheel0.effort[0] = 1.0; //wheel0_current_
//    velocity_publisher_.publish( wheel0 );
    std_msgs::Float64 val;
    val.data = wheel0_current_;
    velocity_publisher_.publish( val );
    ROS_INFO("Leaving sendVal");
  }
}

void WheelControlPad::save( rviz::Config config ) const
{
  rviz::Panel::save( config );
  config.mapSetValue( "Topic", output_topic_ );
}

// Load all configuration data for this panel from the given Config object.
void WheelControlPad::load( const rviz::Config& config )
{
  rviz::Panel::load( config );
  QString topic;
  if( config.mapGetString( "Topic", &topic ))
  {
    output_topic_editor_->setText( topic );
    updateTopic();
  }
}
}

PLUGINLIB_EXPORT_CLASS(bumpybot_wheel_teleop::WheelControlPad, rviz::Panel)
