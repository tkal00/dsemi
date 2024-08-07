#ifndef UTILITY_LOGGER_H
#define UTILITY_LOGGER_H

#include <sstream>
#include <queue>

#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

namespace dsemi {

#define LOG_DEBUG(x)	dsemi::logger::log(x, dsemi::logger::level::LOG_LEVEL_DEBUG);
#define LOG_TRACE(x)	dsemi::logger::log(x, dsemi::logger::level::LOG_LEVEL_TRACE);
#define LOG_INFO(x)		dsemi::logger::log(x, dsemi::logger::level::LOG_LEVEL_INFO);
#define LOG_WARN(x)		dsemi::logger::log(x, dsemi::logger::level::LOG_LEVEL_WARN);
#define LOG_ERROR(x)	dsemi::logger::log(x, dsemi::logger::level::LOG_LEVEL_ERROR);
#define LOG_CRITICAL(x)	dsemi::logger::log(x, dsemi::logger::level::LOG_LEVEL_CRITICAL);

	class logger {
	public:
		enum level {
			LOG_LEVEL_DEBUG,
			LOG_LEVEL_TRACE,
			LOG_LEVEL_INFO,
			LOG_LEVEL_WARN,
			LOG_LEVEL_ERROR,
			LOG_LEVEL_CRITICAL
		};
	public:
		// TODO: change to std::wstring_view
		inline static void log(const std::wstring& msg, level log_level = LOG_LEVEL_TRACE) 
		{
			if (log_level < _log_level)
				return;
			std::async(_on_new_log, msg, log_level);
		}

		inline static void set_level(level minimum_level)
		{
			_log_level = minimum_level;
		}

		inline static level get_level()
		{
			return _log_level;
		}

#pragma region Per Level Logging Functions
		// TODO: change to std::wstring_view
		inline static void debug(const std::wstring& msg) { log(msg, LOG_LEVEL_DEBUG); }
		// TODO: change to std::wstring_view
		inline static void trace(const std::wstring& msg) { log(msg, LOG_LEVEL_TRACE); }
		// TODO: change to std::wstring_view
		inline static void info(const std::wstring& msg) { log(msg, LOG_LEVEL_INFO); }
		// TODO: change to std::wstring_view
		inline static void warn(const std::wstring& msg) { log(msg, LOG_LEVEL_WARN); }
		// TODO: change to std::wstring_view
		inline static void error(const std::wstring& msg) { log(msg, LOG_LEVEL_ERROR); }
		// TODO: change to std::wstring_view
		inline static void critical(const std::wstring& msg) { log(msg, LOG_LEVEL_CRITICAL); }
#pragma endregion

		inline static void start_logger()
		{
			std::wostringstream thread_start_str;
			thread_start_str << L"starting logger | thread_id: ";

			_active = true;
			_thread = std::thread(_thread_begin);

			thread_start_str << _thread.get_id();
			LOG_DEBUG(thread_start_str.str());
		}

		inline static std::thread::id get_id()
		{
			return _thread.get_id();
		}

		inline static void stop_logger() 
		{
			_active = false;

			_cv.notify_all();
			_thread.join();
		}

		// TODO: change to std::wstring_view
		inline static void _on_new_log(const std::wstring& msg, level log_level) {
			// move this to input handler thread?
			_log_queue.emplace(msg, log_level);
			_cv.notify_all();
		}

	private:
		struct log_entry_internal
		{
			log_entry_internal() {}
			// TODO: change to std::wstring_view
			log_entry_internal(const std::wstring& msg, level log_level)
				: msg(msg), log_level(log_level)
			{}

			std::wstring msg;
			level log_level;
		};

	private:
		static void _thread_begin();
		static void _process_queue();
		static void _process_entry(log_entry_internal entry);
		/*inline static void _add_entry(const std::string& msg, level log_level) {
		}*/

	private:
		static std::queue<log_entry_internal> _log_queue;
		static std::string                    _format;
		static std::wostringstream            _stream;

		static level _log_level;

		static std::thread             _thread;
		static std::thread             _notify_thread;
		static bool                    _active;
		static std::mutex              _m;
		static std::condition_variable _cv;
	};
}

#endif
