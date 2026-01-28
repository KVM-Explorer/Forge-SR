module forge.core.profile;

std::unique_ptr<Profiler> Profiler::inst_ = nullptr;
std::unique_ptr<ProfilerConsumer> ProfilerConsumer::inst_ = nullptr;
