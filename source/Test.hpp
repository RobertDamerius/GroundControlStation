#pragma once


class Test {
    public:
        static void Start(void);
        static void Stop(void);

    private:
        static std::thread* threadTest;
        static std::atomic<bool> terminate;
        static void Task(void);
};

