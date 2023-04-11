#include <string>
#include <stdint.h>
#include "../DRAMsim3-master/src/dramsim3.h"
#include "../DRAMsim3-master/src/configuration.h"

class some_object
{
	public: 
		void read_complete(unsigned, uint64_t, uint64_t);
		void write_complete(unsigned, uint64_t, uint64_t);
};
