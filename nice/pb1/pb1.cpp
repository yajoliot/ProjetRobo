#include <avr/io.h>
#include <avr/interrupt.h>

class Nice{
	public:
		uint8_t omgwtf = 0x00;
		uint8_t change(uint8_t new_int){
			yessir = new_int;
		}
	private:
		uint8_t yessir = 0x00;
};

int main(){
	Nice nice;
	nice.omgwtf = 0x01;
	// nice.change(0x02);
}

