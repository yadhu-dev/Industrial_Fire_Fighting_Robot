# Industrial_Fire_Fighting_Robot

An Industrial Fire Detection and Suppression System using ESP8266 fire detection sensor nodes and an ESP32-based autonomous line-following fire fighting robot.

Each node detects flame and smoke and sends a WiFi alert with a Node ID.  
The robot receives the alert, follows a predefined line path, avoids obstacles, reaches the node location, confirms fire, and extinguishes it using a water pump.

---

# Project Images

## Robot Circuit
![Robot](Images/robot.jpeg)

## ZERO PCB Design Front
![Node](Images/pcb1.jpeg)

## ZERO PCB Design Wire Connection
![Node](Images/pcb2.jpeg)


---

# System Architecture

```
ESP8266 NODE 1          ESP8266 NODE 2
(Flame + MQ-2)          (Flame + MQ-2)
       |                        |
       -------- WiFi TCP -------
                    |
               ESP32 ROBOT
                    |
     ---------------------------------
     | IR Line | Ultrasonic | Flame  |
     | Sensor  |  Sensor    | Sensor |
     ---------------------------------
                    |
             L298N Motor Driver
                    |
                BO Motors
                    |
                Water Pump
                    |
                  Buzzer
```

---

# Working Principle

1. ESP8266 node detects:
   - Flame OR
   - Smoke above threshold
2. Node sends WiFi message:

```
Node(1/2):FIRE
```

3. ESP32 receives Node ID  
4. Robot selects mapped line path  
5. Robot follows line to target node  
6. Ultrasonic avoids obstacles  
7. Robot stops near fire location  
8. Flame sensor confirms fire  
9. Water pump activates   

---

# Communication Method

| Parameter | Value |
|-----------|-------|
Protocol | TCP over WiFi |
Server | ESP32 |
Clients | ESP8266 nodes |
Port | 80 |
Message Format | NODE_ID:STATUS |
Example | Node2:FIRE |

## ESP32 Parsing Logic

```cpp
String data = clients[i].readStringUntil('\n');
data.trim();

int id = data.substring(0, 1).toInt();
String msg = data.substring(2);
```

---

# Line Path Mapping

| Node ID | Robot Path |
|---------|-----------|
1 | Straight → Left → Stop |
2 | Straight → Right → Stop |
3 | Straight → Straigh → Left → Stop |
4 | Straight → Straigh → Right → Stop |

---

# Hardware Components

## ESP8266 Fire Nodes (x2)

- ESP8266  
- MQ-2 Gas/Smoke Sensor  
- IR Flame Sensor  
- Zero PCB  
- Female Headers  
- Micro USB Cable  

## ESP32 Fire Fighting Robot

- ESP32  
- BO Motors x2  
- BO Wheels x2  
- Ball Caster Wheel  
- L298N Motor Driver  
- IR Line Sensors x2  
- Ultrasonic Sensor (HC-SR04)  
- IR Flame Sensor  
- Mini DC Water Pump  
- Relay Module  
- Servo Motor (Nozzle control) 

---

# Power System

- 3.7V 6000mAh Li-ion Battery x2 (7.4V output)  
- TP5000 Charging Module  
- LM7805 Voltage Regulator (5V logic supply)

## Power Flow

```
Battery (7.4V)
   |-- L298N Motor Driver -> BO Motors
   |-- Relay -> Water Pump
   |-- LM7805 -> ESP32 + Sensors + Servo 
```

---

# Getting Started

## 1. Upload Node Code

- Set Node ID  
- Enter WiFi SSID and Password  
- Set smoke threshold  
- Upload to ESP8266  

## 2. Upload Robot Code

- Configure WiFi credentials  
- Set path mapping for each Node ID  
- Upload to ESP32  

## 3. Power the System

- Connect Li-ion battery pack  
- Ensure 5V output from LM7805  
- Test pump using relay  

---

# Testing Procedure

- Test ESP8266 WiFi message transmission  
- Verify ESP32 receives Node ID  
- Test line following independently  
- Test ultrasonic obstacle avoidance  
- Trigger node and verify robot reaches correct path  
- Test flame detection and pump activation  

---

# Safety Notes

- Use controlled flame only during testing  
- Add manual emergency stop switch  
- Isolate high-current pump wiring  
- Ensure proper grounding  

---

# Future Enhancements

- Multi-node priority handling for simultaneous fire alerts  
- Thermal camera based fire detection for higher accuracy  
- Custom IoT dashboard for live monitoring (self-developed software, no third-party platform)  
- Real-time sensor data logging and event history storage  
- AI/ML-based fire classification (flame vs smoke vs false trigger)  
- AI-based path optimization for fastest robot response  
- Predictive maintenance using motor current and battery health data  
- SLAM-based navigation without line following  
- Autonomous docking and automatic battery charging  
- Wireless firmware updates for ESP nodes and robot 

---

# Academic Details

Project Title: Industrial Fire Fighter Rover  
Course: M.Sc. Instrumentation Science – Semester II  
University: Savitribai Phule Pune University  
Department: Electronic and Instrumentation Science  

---

# Team Members

- Tank Neelkumar 
- Abhishek Karande  
- Pallavi Pawase
- Kiran Zende 
- Pritom Manna  
- Srushit Rahane    
- Kamini Shinde
- Rakesh M R 
- Yadhul Mohan   

---
