#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>
#include <uav_oc_msgs/UAVOptCtlPolicy.h>

namespace mavros {
namespace extra_plugins {

class OptCtlPlugin : public plugin::PluginBase {
public:
    OptCtlPlugin() : plugin::PluginBase(),
    optctl_nh("~optctl")
    { }

    void initialize(UAS &uas_)
    {
        PluginBase::initialize(uas_);
        optctl_sub = optctl_nh.subscribe("/optctl/actuator_control", 10, &OptCtlPlugin::policy_cb, this);

    }

    Subscriptions get_subscriptions()
	{
		return {};
	}

private:
    ros::NodeHandle optctl_nh;
    ros::Subscriber optctl_sub;

    void policy_cb(const uav_oc_msgs::UAVOptCtlPolicyConstPtr &msg)
    {
        mavlink::common::msg::OPTCTL_POLICY policy_data;

        std::array<float, 4> ffterm = {msg->ffterm.mx, msg->ffterm.my, msg->ffterm.mz,msg->ffterm.th};
        policy_data.ffterm = ffterm;

        UAS_FCU(m_uas)->send_message(policy_data);



    }
};
} // namespace extra_plugins
} // namespace mavros

PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::OptCtlPlugin, mavros::plugin::PluginBase)