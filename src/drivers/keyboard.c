#include "keyboard.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

static char key_buffer[256];

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", "7", 
  "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
  "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
  "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
  "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
  "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', 
  '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
  'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
  'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
  'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

static void keyboard_callback(registers_t regs) {
  /* The PIC leaves us the scancode in port 0x60 */
  u8 scancode = port_byte_in(0x60);
	
	// Nothing to be done for touch up and undefined touch downs.
	if(scancode > SC_MAX) {
		return;
	}

	if(scancode == BACKSPACE) {
		if(strlen(key_buffer)) {
			backspace(key_buffer);
			printk_backspace();
		}
	}
	else if(scancode == ENTER) {
		printk("\n");
		user_input(key_buffer);
		clear(key_buffer);
	}
	else {
		char key_letter = sc_ascii[(int)scancode];
		char str[2] = {key_letter, '\0'};
		//Append the key to the input buffer
		append(key_buffer, str);
		printk(str);
	}
}

void init_keyboard() {
  register_interrupt_handler(IRQ1, keyboard_callback); 
}
