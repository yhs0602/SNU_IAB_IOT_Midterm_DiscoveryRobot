# SNU_IAB_IOT_Midterm_DiscoveryRobot
A short term project for SNU IAB IoT class midterm. This is a software (arduino sketch) for a human-searching disaster help robot.

## Indoor positioning
1. It scans for WiFi APs around it and calculate distance using RSSI.
1. Repeat `1.` three times at different positions.
1. Calculte APs' relative positions.
1. From now on, repeat `1.` to find itself's position, assuming that the APs are in fixed positions.

## Supersonic sensors
1. It helps the positioning process to be more accurate.
1. The sensor heading above analyzes the ceiling over it.

## Human detection
1. It has a human detecting sensor to help a person notify admin that they are there.

## Optical Camera
1. The QQVGA camera video frames are streamed via WiFi websocket, and displayed on the admin's browser. 
