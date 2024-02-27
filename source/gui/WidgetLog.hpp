#pragma once


#include <nanogui.h>
#include <LogEntry.hpp>


class WidgetLog: public nanogui::Window {
    public:
        /**
         *  @brief Create a log widget.
         *  @param [in] parent The parent widget.
         */
        explicit WidgetLog(nanogui::Widget *parent);

        /**
         *  @brief Delete the log widget.
         */
        ~WidgetLog();

        /**
         *  @brief Update the log elements.
         */
        void UpdateLogs(void);

        /**
         *  @brief Add a new log entry (thread-safe).
         *  @param [in] log The log to be added.
         */
        void AddLogEntry(const LogEntry& log);

        /**
         *  @brief Clear the whole log.
         *  @details This will only set the requestClearLogs flag. The actual clearing is done by the next @ref UpdateLogs call.
         */
        void ClearLog(void);

    private:
        nanogui::Widget* logContent;     ///< The widget where to add new logs.
        std::vector<LogEntry> logsToAdd; ///< The logs that should be added.
        std::mutex mtx;                  ///< Protect @ref logsToAdd.
        bool requestClearLogs;           ///< True if logs should be cleared, false otherwise.
};

