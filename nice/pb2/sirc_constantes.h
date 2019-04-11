volatile uint8_t count = 0x00;
volatile uint16_t overflow_count = 0x0000;

volatile bool edge_detected = false;
enum whose{SENDER, RECEIVER};
volatile whose turn = SENDER;
volatile uint16_t count_ = 0x0000;
