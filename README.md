<div style="font-family: Arial, sans-serif; max-width: 900px; margin: 20px auto; line-height: 1.6; color: #2c3e50;">
  <!-- Title -->
  <h1 style="text-align: center; font-size: 36px; color: #34495e; font-weight: bold;">Underpass Water Logging Detection System</h1>

  <!-- Overview Section -->
  <div style="background: #f4f6f8; border-radius: 8px; padding: 20px; margin: 20px 0; box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);">
    <h2 style="font-size: 24px; color: #2c3e50; margin-bottom: 15px;">Overview</h2>
    <p style="font-size: 18px;">
      This project implements an IoT-based solution to monitor water levels in underpasses during heavy rainfall. The system ensures safety by activating alarms, controlling gates and pumps, and updating real-time water level data to Firebase. A React-based dashboard visualizes the data for easy monitoring and decision-making.
    </p>
  </div>

  <!-- Features Section -->
  <div style="background: #ffffff; border-radius: 8px; padding: 20px; margin: 20px 0; box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);">
    <h2 style="font-size: 24px; color: #2c3e50; margin-bottom: 15px;">Features</h2>
    <ul style="font-size: 18px; list-style-type: square; padding-left: 20px;">
      <li>Real-Time Water Level Monitoring: Uses a water level sensor connected to an ESP32 to measure water levels in real-time.</li>
      <li>Automatic Alert System: Activates a buzzer when water levels exceed a critical threshold (e.g., 60 cm).</li>
      <li>Gate and Pump Control: Automatically closes the underpass gate and activates a water pump to drain water.</li>
      <li>Data Visualization: Real-time and historical water level data displayed on a React dashboard with charts and filters.</li>
      <li>Firebase Integration: Stores water level data with timestamps for analysis and monitoring.</li>
    </ul>
  </div>

  <!-- Technology Stack Section -->
  <div style="background: #eef1f6; border-radius: 8px; padding: 20px; margin: 20px 0; box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);">
    <h2 style="font-size: 24px; color: #2c3e50; margin-bottom: 15px;">Technology Stack</h2>
    <ul style="font-size: 18px; list-style-type: disc; padding-left: 20px;">
      <li><strong>Hardware:</strong> ESP32, water level sensor, relays, buzzer, water pump.</li>
      <li><strong>IoT Protocols:</strong> Wi-Fi for ESP32 connectivity.</li>
      <li><strong>Backend:</strong> Firebase Realtime Database for storing and retrieving data.</li>
      <li><strong>Frontend:</strong> React for the dashboard interface.</li>
      <li><strong>Programming Languages:</strong> C++ (Arduino), JavaScript (React).</li>
    </ul>
  </div>

  <!-- How It Works Section -->
  <div style="background: #ffffff; border-radius: 8px; padding: 20px; margin: 20px 0; box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);">
    <h2 style="font-size: 24px; color: #2c3e50; margin-bottom: 15px;">How It Works</h2>
    <ol style="font-size: 18px; list-style-type: decimal; padding-left: 20px;">
      <li>
        <strong>Water Level Measurement:</strong> The water level sensor measures water depth and sends analog readings to the ESP32.
      </li>
      <li>
        <strong>Data Processing:</strong> The ESP32 processes sensor data, tracks the highest and lowest water levels, and applies thresholds for actions like activating alarms or pumps.
      </li>
      <li>
        <strong>Firebase Integration:</strong> Real-time data is uploaded to Firebase Realtime Database with timestamps.
      </li>
      <li>
        <strong>Alerts and Automation:</strong>
        <ul style="list-style-type: circle; padding-left: 20px;">
          <li>Activates a buzzer when water exceeds critical levels.</li>
          <li>Closes gates and turns on pumps to prevent waterlogging.</li>
        </ul>
      </li>
      <li>
        <strong>Visualization:</strong> The React dashboard visualizes water levels (current, highest, and lowest) and allows users to filter data by daily, weekly, or monthly views.
      </li>
    </ol>
  </div>

  <!-- Installation and Setup Section -->
  <div style="background: #f4f6f8; border-radius: 8px; padding: 20px; margin: 20px 0; box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);">
    <h2 style="font-size: 24px; color: #2c3e50; margin-bottom: 15px;">Installation and Setup</h2>
    <ol style="font-size: 18px; list-style-type: decimal; padding-left: 20px;">
      <li>
        <strong>Prerequisites:</strong>
        <ul style="list-style-type: circle; padding-left: 20px;">
          <li>Install the latest version of Arduino IDE.</li>
          <li>Install the following libraries: Firebase ESP32 Client, NTP Client, WiFi.h, ESP32 board drivers in Arduino IDE.</li>
          <li>Set up a React environment for the dashboard.</li>
        </ul>
      </li>
      <li>
        <strong>Hardware Setup:</strong> Connect the water level sensor, relays, and buzzer to ESP32 as per the hardware connections.
      </li>
      <li>
        <strong>Code Configuration:</strong> Replace placeholder credentials with your Wi-Fi and Firebase project details in the main.ino file.
      </li>
      <li>
        <strong>Firebase Realtime Database Structure:</strong> Ensure your database structure matches the project requirements.
      </li>
    </ol>
  </div>
</div>
