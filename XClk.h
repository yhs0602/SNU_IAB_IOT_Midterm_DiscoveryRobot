/* Author : Mudassar Tamboli
 * License : APACHE LICENSE
 * URL: https://github.com/mudassar-tamboli/ESP32-OV7670-WebSocket-Camera/blob/master/XClk.h
 * DESC: 비디오 폴링용 클럭을 위한 헤더 파일?
 */
 
#pragma once

bool ClockEnable(int pin, int Hz);
void ClockDisable();
