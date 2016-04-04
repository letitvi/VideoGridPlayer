// VGP_ConsoleTest.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "vgp/vgp.hpp"
#include <thread>
#include <chrono>

int main()
{
    using namespace std::chrono_literals;

    vgp::NewVideoParam newParam;
    vgp::ControlVideoParam controlParam;

    vgp::VideoGridPlayer player(800, 600, "test");
    player.NewVideo(1, newParam);

    
    while (1)
    {
        std::this_thread::sleep_for(50ms);
    }
    return 0;
}

