#include "cow.h"


void Cow::setTemperature(float temp){
    if (temp > 15.0) {
        temperature = temp;
    } 
}

float Cow::getTemperature() {
    return temperature;
}

void Cow::inputImu(int x, int y) {
    imu[count % 3] = x + y;
    count++;
}

int Cow::getState() {   //1반환시, stable 한 상태
    
    for (int i = 0; i < 3; i++) {
        if (imu[i] < 2500)
            state = 1;
        
        else {
            state = 0;
            break;
        }  
    }
    return state; 
   
}