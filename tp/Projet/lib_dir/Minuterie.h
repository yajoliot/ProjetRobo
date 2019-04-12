#ifndef LIB_DIR_MINUTERIE_H
#define LIB_DIR_MINUTERIE_H

#include <stdlib.h>
#include <avr/io.h> 



	/**
 	 * This class RUNS only on TIMER2
 	 */
	class MINUTERIE{
		public:
			MINUTERIE(minuterie_configuration configuration_);
			~MINUTERIE();
			void startMinuterie(uint16_t) duree;
			void stopMinuterie();
			void resetMinuterie();
		private:
			minuterie_configuration* configuration;

	};

#endif