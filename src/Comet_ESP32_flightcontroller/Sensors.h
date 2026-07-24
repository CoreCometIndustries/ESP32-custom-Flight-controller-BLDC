#ifndef SENSORS_H
#define SENSORS_H

void detectIMU();
void detectBaro();
void detectMag();
void configureIMU();
void readIMU();
void applyCalibration();
void updateAttitudeFilter();

#endif // SENSORS_H
