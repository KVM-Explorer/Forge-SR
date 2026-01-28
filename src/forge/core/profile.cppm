export module forge.core.profile;

import std;

struct ProfileEvent {
    std::string_view name;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
    float duration_ms;
};

struct ProfilerData {
    static const int FilterSize = 32;
    std::array<ProfileEvent, FilterSize> m_event_filter{};
    int m_event_count = 0;
    int m_cur_index = 0;

    float avg_ms;
    float min_ms;
    float max_ms;
};

using Task = std::move_only_function<void()>;

class ProfilerConsumer {
    static std::unique_ptr<ProfilerConsumer> inst_;
    std::queue<Task> m_tasks;
    std::mutex m_mtx;
    std::condition_variable m_cv;
    bool m_stop = false;

    std::unordered_map<std::string_view, ProfilerData> m_data;

  public:
    static ProfilerConsumer &Get() {
        if (!inst_) {
            inst_ = std::make_unique<ProfilerConsumer>();
        }
        return *inst_;
    }

    ~ProfilerConsumer() {
        {
            std::lock_guard lock(m_mtx);
            m_stop = true;
        }
        m_cv.notify_all();
    }
    void worker_thread() {
        while (true) {
            Task task;
            {
                std::unique_lock lock(m_mtx);
                m_cv.wait(lock, [this] { return m_stop || !m_tasks.empty(); });
                if (m_stop && m_tasks.empty())
                    return;

                task = std::move(m_tasks.front());
                m_tasks.pop();
            }
            task(); // 在线程 B 中执行
        }
    }

    // 统计线程将数据传输到消费者
    static void dispatch(Task task) {
        auto &inst = Get();
        {
            std::lock_guard lock(inst.m_mtx);
            inst.m_tasks.push(std::move(task));
        }
        inst.m_cv.notify_one();
    }

    void put(std::vector<ProfileEvent> &events) {
        // 将事件数据放入 ProfilerData 中
        for (auto &event : events) {
            auto &data = m_data[event.name];

            // 更新环形缓冲区
            data.m_event_filter[data.m_cur_index] = event;
            data.m_cur_index = (data.m_cur_index + 1) % ProfilerData::FilterSize;
            data.m_event_count = std::min(data.m_event_count + 1, ProfilerData::FilterSize);

            // 计算统计数据
            float sum = 0.0f;
            for (int i = 0; i < data.m_event_count; ++i) {
                sum += data.m_event_filter[i].duration_ms;
                data.min_ms = std::min(data.min_ms, event.duration_ms);
                data.max_ms = std::max(data.max_ms, event.duration_ms);
            }
            data.avg_ms = sum / data.m_event_count;
        }
    }
};

class Profiler {

  private:
    static std::unique_ptr<Profiler> inst_;
    static constexpr int MaxEvents = 64;
    int assigned_id_ = 0;
    std::array<ProfileEvent, MaxEvents> events_;

  public:
    static Profiler &Get() {
        if (!inst_) {
            inst_ = std::make_unique<Profiler>();
        }
        return *inst_;
    }

    int begin(std::string_view name) {
        // 分配event ID
        int event_id = assigned_id_++;

        // 记录开始时间点
        events_[event_id].start_time = std::chrono::high_resolution_clock::now();
        events_[event_id].name = name;
        return event_id;
    }

    void end(int event_id) {
        events_[event_id].end_time = std::chrono::high_resolution_clock::now();
        events_[event_id].duration_ms =
            std::chrono::duration_cast<std::chrono::microseconds>(events_[event_id].end_time - events_[event_id].start_time).count() / 1000.0f;
    }

    void submit() {
        std::vector<ProfileEvent> submitted_events;
        submitted_events.reserve(assigned_id_);
        submitted_events.insert(submitted_events.end(), events_.begin(), events_.begin() + assigned_id_);
        assigned_id_ = 0; // 重置分配 ID

        ProfilerConsumer::dispatch([submitted_events = std::move(submitted_events)]() mutable {
            // 在消费者线程中处理提交的事件
            ProfilerConsumer::Get().put(submitted_events);
        });
    }
};

class ScopedProfiler {

    int event_id_ = -1;

  public:
    ScopedProfiler(std::string_view name) { event_id_ = Profiler::Get().begin(name); }

    ~ScopedProfiler() { Profiler::Get().end(event_id_); }
};

enum class TimerMode { Raw, Scoped };
class RawTimer {
  public:
    RawTimer(std::string name, TimerMode mode = TimerMode::Raw) {
        if (mode == TimerMode::Scoped) {
            // start scoped timer
            start_time_ = std::chrono::high_resolution_clock::now();
        }
    }

    std::string_view GetName() { return name_; }

    void begin() {
        // start raw timer
        start_time_ = std::chrono::high_resolution_clock::now();
    }

    void end() {
        // stop raw timer
        end_time_ = std::chrono::high_resolution_clock::now();
        duration_ = std::chrono::duration_cast<std::chrono::microseconds>(end_time_ - start_time_);
        std::print("Timer [{}] took {} microseconds\n", name_, duration_.count());
    }

    float get_duration_ms() { return duration_.count() / 1000.0f; }

    ~RawTimer() {
        if (mode_ == TimerMode::Scoped) {
            // stop scoped timer
            end_time_ = std::chrono::high_resolution_clock::now();
            duration_ = std::chrono::duration_cast<std::chrono::microseconds>(end_time_ - start_time_);
            std::print("Timer [{}] took {} microseconds\n", name_, duration_.count());
        }
    }

  private:
    std::string name_;
    TimerMode mode_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time_;
    std::chrono::microseconds duration_;
};
