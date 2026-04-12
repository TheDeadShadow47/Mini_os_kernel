#include "port.h"

void port_wait() {
	port_inb(0x80);
}