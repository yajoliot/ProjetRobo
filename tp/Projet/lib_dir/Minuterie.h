#ifndef LIB_DIR_MINUTERIE_H
#define LIB_DIR_MINUTERIE_H

#include <stdlib.h>

namespace minuterie{

	/**
	 * Minuterie modes 
	 *  - CTC = clear the counter
	 *  - PFC = phase and frequency correct
	 */
	enum minuterie_mode{CTC,PFC};

	/**
	 * Prescaler modes
	 *   - 1
	 *   - 8
	 *   - 256
	 *   - 1024
	 */
	enum prescaler_mode{p1=1,p2=8,p3=256,p4=1024};

	/**
	 * Output mode
	 *  - No pin
	 *  - 1 pin (your input decides which one)
	 *  - 2 pin
	 */
	enum output_mode{noPin,onePin,twoPin};

	/**
	 * Interrupt mod
	 *  - OFF
	 *  - OVF (counter overflow)
	 */
	enum interrupt_mode{off,ovf};

	/**
	 * Initialiser data structure for minuterie
	 *  - minuterie_mode and prescaler_mode always need to be defined in order for the minuterie to work
	 *  - this data structure will be saved and persist unless destroyed by ~MINUTERIE()
	*   - this data structure will always be loaded as the configuration if it exists after enableMinuterie is called
	*/
	typedef struct{
		minuterie_mode minuterie_mode_;
		prescaler_mode prescaler_mode_;
		output_mode output_mode_;
		interrupt_mode interrupt_mode_;
	}minuterie_configuration;


	/**
 	 * This class RUNS only on TIMER1
 	 */
	class MINUTERIE{
		public:
			MINUTERIE(minuterie_configuration configuration_);
			~MINUTERIE();
			startMinuterie(uint16_t);
			stopMinuterie();
			resetMinuterie();
		private:
			minuterie_configuration* configuration;

	};
}
#endif