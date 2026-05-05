#pragma once

#include "common/sys/rc.h"
#include "sql/executor/command_executor.h"

class SQLStageEvent;

class DropTableExecutor : public CommandExecutor
{
public:
  RC execute(SQLStageEvent *sql_event) override;
};
