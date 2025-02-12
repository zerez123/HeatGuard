# HeatGuard

## Overview
HEATGUARD is an Arduino-based safety system designed to prevent fire hazards caused by fallen space heaters. It monitors the heater's orientation using an accelerometer and cuts off power if the device is not in the correct position.

## Features
- **Real-time monitoring** of the heater's position using an accelerometer.
- **Automatic power cutoff** when the heater falls to prevent fire hazards.
- **Low-power operation** with efficient Arduino firmware.
- **Customizable settings** to adjust sensitivity and response time.

## Hardware Requirements
- **Arduino Nano Board** (Can use other arduino board)
- **ADXL355 Accelerometer**
- **Relay module** 
- **Power supply** (depending on relay and heater requirements)

## Software Requirements
- **VS code with PIO** (latest version recommended)


## Installation & Usage
1. **Clone the repository**:
   ```sh
   git clone https://github.com/zerez123/HeatGuard.git
   ```
2. **Open the project in VS IDE**.
4. **Connect the hardware components** as per the circuit diagram.
5. **Upload the firmware** to the Arduino board.
6. **Test the system** by tilting the heater and verifying power cutoff.

## Configuration
Modify the following parameters in header.h to adjust system behavior:
- **ACCMOVETHR**: Set the angle at which the power cutoff is triggered.
- **SLEEPTIME**: Adjust the polling time of the system.

## Safety Considerations
- Ensure the relay can handle the heater's power load.
- Use proper insulation and enclosures to prevent electric hazards.
- Test thoroughly before deploying in a real environment.

## Contributing
Contributions are welcome! Feel free to open issues and submit pull requests.

## License
This project is licensed under the **GPLv3**. See the `LICENSE` file for details.


