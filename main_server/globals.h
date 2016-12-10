#ifndef GLOBALS_H
#define	GLOBALS_H
#include "arduino.h"
#include "asynctcp.h"
#include <iostream>
class arduino;

extern std::vector<std::shared_ptr <arduino>> ard;
extern std::vector<tcp_session *> cli_stream_duty;
extern std::vector<tcp_session *> cli_stream_lux;

#endif //GLOBALS
