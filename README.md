# MIDI Sequencer

MIDI sequencer for the STM32F103C8T6 "Blue Pill" with SSD1306 128x64 OLED display. Menu-driven UI using two pushbutton rotary encoders.

---

## Table of Contents - [Features](#features) - [Hardware Requirements](#hardware-requirements) - [Schematic](#schematic) - [STM32 Blue Pill Pinout Configuration](#stm32-blue-pill-pinout-configuration) - [3D PCB Render](#3d-pcb-render) - [Project Structure](#project-structure) - [Building and Flashing](#building-and-flashing) - [STM32F103 Setup (CubeMX)](#stm32f103-setup-cubemx) - [Hardware](#hardware) - [Firmware Architecture](#firmware-architecture)

---

## Features

- **Menu-driven sequencer** via two pushbutton rotary encoders
- **Simple main.c note generator engine** driven by TIM4 timer interrupts
- **OLED display output** using 128 x 64, .96" SSD1306 over I2C
- **MIDI note generation** displayed real-time on OLED display
- **13 common modes** including 5, 6 and 7 interval sequences 
- **User interface with rotary encoders** enables customized settings for mode, key, octave range, tempo and channel selection
- **Console UART** for debug messaging and session monitoring
- **MIDI Out buffering** with activity LED

---

## Hardware Requirements

- STM32F103C8T6 ("Blue Pill")
- SSD1306 128x64 OLED (I2C)
- Rotary encoders with push buttons
- MIDI OUT circuit (UART Tx buffering and 220 ohm current limiting resistor)
- ST-Link v2 or compatible programmer/debugger

---

## Schematic

![alt text](image-2.png)

---

## STM32 Blue Pill Pinout Configuration

![alt text](image-1.png)


---

## 3D PCB Render

![3D PCB View](hardware/midi_traffic_monitor_allegro_3d_view_isometric_Rev_A.PNG)


---

## Project Structure

```text
MIDI_Traffic_Monitor/
├── Core/                   # Source code and header files
├── Drivers/                # STM32Cube HAL/LL drivers
├── Debug/                  # Build output (ignored by Git)
├── hex_image/              # Prebuilt hex image for flashing STM32F103
├── hardware/               # Schematic (pdf), gerbers (zipped), 3D render
├── midi_monitor.ioc        # STM32CubeMX configuration
├── STM32F103C8TX_FLASH.ld
├── .gitignore
├── .project
├── .cproject
├── README.md
```

## Building and Flashing
1. Open the project in STM32CubeIDE.
2. Build the project (`Project > Build All`).
3. Flash to target using ST-Link (`Run > Debug As > STM32 Cortex-M C/C++ Application`).
4. Or flash the prebuilt image from `hex_image/` using STM32CubeProgrammer.

---

## STM32F103 Setup (CubeMX)

- STM32F103C8T6
    - Flash - 128k, SRAM - 20k
- STM32F103 configuration files generated from CubeMX:
    - USART1 - MIDI Out (PA9), 31,250 baud
    - USART2 - Console UART (PA2/PA3), 115,200 baud
    - SSD1306 OLED I2C Display - SCL1/SDA1 (default PB6/PB7)
    - Value Selection Rotary Encoder - TIM2_CH1/CH2 (PA0/PA1), Encoder Interface Mode (Quadrature Mode)
        - Combined Channels = Encoder Mode
        - Encoder Mode - TI1 and TI2
        - GPIO - Input with pull-up
        - Falling Edge, Input Filter ON (filter strength set to 4)
    - Value Selection Pushbutton - GPIO PA4 (Input w/pull-up, EXTI mode, rising/falling edge)
    - Menu Selection Rotary Encoder - TIM3_CH1/CH2 (PA6/PA7), Encoder Interface Mode (Quadrature Mode)
        - Combined Channels = Encoder Mode
        - Encoder Mode - TI1 and TI2
        - GPIO - Input with pull-up
        - Falling Edge, Input Filter ON (filter strength set to 4)
    - Menu Selection Pushbutton - GPIO PA5 (Input w/pull-up, EXTI mode, rising/falling edge)
    - SWD Interface - PA13, PA14 (Blue Pill)
    - User LED (Blue Pill) - PC13
    - TIM4 - one-shot timer w/interrupt (adjustable) - controls MIDI note generation timing
        - One Pulse Mode
        - Prescaler = 64800 - 1, Counter Period = adjustable for BPM note timing
    - RCC Crystal Oscillator (HSE - 8 MHz) - PD0/PD1 (default on STM32 Blue Pill)
    - Clock Configuration - HSE enabled (8 MHz), PLL 9x, SysClk = 72 MHz
    - SYSTICK Timer - default 1 ms
    - NVIC:
        - EXTI line4 interrupt enabled, priority 0
        - EXTI line[9:5] interrupts enabled, priority 0
        - TIM4 global interrupt enabled, priority 0

---
## Hardware

- SSD1306 OLED display:
    - 128x64 pixels, I2C interface w/on-board pullups
    - UI design ... status line at top, main screen holds 6 MIDI records
- MIDI Out
    - MIDI UART Tx buffered by 74LS04 Hex Inverter
        - Drives MIDI Out connector and MIDI Activity LED
- Value Selection Rotary Encoder
    - 2 counts/detent, 30 detents/revolution
        - "Smoother" feel for precise control
    - TIM2 counter divided by 2 yields 1 increment (positive or negative) per detent
- Menu Selection Rotary Encoder
    - 4 counts/detent, 20 detents/revolution
        - "Purposeful" feel for menu selection
    - TIM3 counter divided by 4 yields 1 increment (positive or negative) per detent
- Encoder Pushbuttons
    - GPIO Input w/pull-up, EXTI mode, rising/falling edge
    - External pullups (1k) added to increase/improve button-release rise time
- Console UART
    - Standard/typical 115,200 console interface
    - __io_putchar() defined in main.c to support printf debugging
- Rev A - added hardware MIDI Thru/MIDI Out selection jumper to enable MIDI Out capability
    - Input to 74LS04 is jumper-selectable:
        - Route STM32 USART1 Tx to MIDI Thru connector (for MIDI Out functionality)

---

## Firmware Architecture

- No RTOS, simple SYSTICK-based task scheduler (1ms tick resolution)
    - Add `HAL_SYSTICK_IRQHandler();` to the `USER CODE BEGIN SysTick_IRQn 0` section of `void SysTick_Handler(void)` in `stm32f1xx_it.c`
        - This ensures `HAL_SYSTICK_Callback()` is called every 1ms
        - Note - `HAL_SYSTICK_IRQHandler()` is defined in `stm32f1xx_hal_cortex.c`
    - In `HAL_SYSTICK_Callback()` (in `main.c`), add:
        - `ms_counter++;` to track elapsed time
        - `run_scheduled_tasks();` to dispatch tasks at their scheduled intervals
    - Task setup is defined in `tasks.c`:
        - Use `scheduler_add_task()` in `tasks_init()` to register tasks
        - Define task callback functions (e.g., `heartbeat_task()`, `read_encoders_task()`, `poll_buttons_task()`)
    - In `main.c`, call `scheduler_init();` and `tasks_init();` before the `while(1)` loop
    - The scheduler is driven by `run_scheduled_tasks()` called from `HAL_SYSTICK_Callback()`
    - Currently, 3 tasks defined:
        - `heartbeat` – toggles LED for system heartbeat
        - `read_encoders` – reads rotary encoder state
        - `poll_buttons` – checks button inputs
    - **Troubleshooting Tip**: If tasks aren't running or the heartbeat LED doesn't blink, ensure `HAL_SYSTICK_IRQHandler();` is present in `SysTick_Handler()`. Without it, `HAL_SYSTICK_Callback()` won't be triggered.


- MIDI UART (USART1)
    - Sequencer note generation from USART1 Tx:
        - HAL_UART_Transmit(&huart1, midi_note_packet, sizeof(midi_note_packet), 100);

- Console UART (USART2)
    - Add `__io_putchar()` in main.c to support printf debugging:
        - int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

- Encoder Pushbutton Handlers
    - Interrupt based:
        - Rising and falling edge detection supports and identifies press and release events
    - Short/long press events and actions supported:
        - "Long" duration #defined in buttons.c :
            - LONG_PRESS_THRESHOLD_MS 500
        - Menu Selection pushbutton:
            - Short = start/stop sequencer
            - Long = none
        - Value Selection pushbutton:
            - Short = move selection highlight to parameter of interest
            - Long = none

- Simple main.c forevever loop:
    - Checks sequencer on/off setting/flag and TIM4 timeout
        - selects random note from note selection list, writes MIDI packet to UART
        - updates OLED display with note being sent

- tasks.c
    - Processes scheduler-dispatched periodic tasks
        - heartbeat() task - 500 ms
            - Routine activity ... flashes heartbeat LED
            - Animates (blinks) multi-value selection menu item
        - read_encoders() task - 20 ms
            - Reads TIM2/TIM3 counters, compares current to previous counts to detect movement
            - Calculates delta and calls UI encoder handlers
        - poll_buttons() task - 10 ms
            - Calls button_poll() in buttons.c
            - Processes button events decoded and posted in button_poll()
                - Note - button_exti_trigger(), called by HAL_GPIO_EXTI_Callback() in stm32f1xx_it.c, handles debounce and updates button states that are then used by button_poll() to determine actions
            - Switch statement gets button event from button_get_event() in buttons.c

- ui.c
    - Main user interface handler
    - Manages Menu Selection and Value Selection user inputs with switch statements
    - Main menu system defined by X-Marco definition in ui_menu_def.h
        - X-macro expansion defines menu headings that will be used to generate enum and const char * array
        - Switch statement uses X-macro enums for case labels which simplifies menu changes/additions
    - Sub-menus also driven by switch statement with enum case labels

- ssd1306.c
    - Library from https://github.com/afiskon/stm32-ssd1306/tree/master
    - Font - Font_6x8
    - 7 lines, 21 characters per line

- display.c
    - Display layout (7 usable lines):
        - Status Line - top line reserved for status and menu information
        - Main Screen - remaining 6 lines for real-time MIDI note display
    - Helper functions for SSD1306 display
        - display_clear_screen() - clears entire screen
        - display_clear_page() - clears main screen (scroll bar and status line unaffected)
        - display_string() - display string to main screen
        - display_status() - displays status message to status line
        - display_string_to_status_line() - display free-form string to status line
        - display_splash_screen() - just cosmetics, displays application statistics

- midi.c
    - Builds MIDI packets for OLED display:
        - midi_process_message()
    - Converts MIDI note number (0–127) to string like "C4"
        - midi_get_note_name()

