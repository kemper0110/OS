#pragma once
#include "Battleship.h"
#define LEAN_AND_MEAN
#include <Windows.h>
#include "perror.h"


class PipeBattleship :
    public Battleship
{
    HANDLE pipe;
public:
    PipeBattleship();
    // Унаследовано через Battleship
    virtual bool waitBeginning() override;
    virtual void send(const Message& message) override;
    virtual Message receive() override;
};

