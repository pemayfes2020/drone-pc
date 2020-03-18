#include "safe_exit.hpp"

#include <chrono>
#include <iostream>
#include <thread>

// コンストラクタとデストラクタが呼ばれることを確認するだけのクラス
class A
{
public:
    A()
    {
        std::cout << "constructor" << std::endl;
    }

    ~A()
    {
        std::cout << "destructor" << std::endl;
    }
};

void work()
{
    // スレッド内でenterする
    ThreadRoom::enter();
    try {
        // 寿命を適切に管理したい変数をtry節中で宣言する
        A a;
        while (!ThreadRoom::toExit()) {
            auto start = std::chrono::system_clock::now();
            while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() < 1000) {
            }
            std::cout << "hello" << std::endl;
        }
    } catch (ThreadRoom::thread_abort) {
        // toExit() が例外を投げるので、try節中の変数の寿命を断ったのちexitする(残りスレッド数を1減らす)
        ThreadRoom::exit();
        return;
    }

    return;
}

int main()
{
    ThreadRoom::setExitHandler();

    std::thread{work}.detach();
    std::thread{work}.detach();
    std::thread{work}.join();

    // 無限ループが走るので、Ctrl+CでSIGINTを与えると正常にdestructorが3回呼ばれて落ちる

    return 0;
}
