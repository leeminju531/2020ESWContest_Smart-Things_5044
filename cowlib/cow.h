#ifndef COW
#define COW
class Cow{
    
public:
        
        void setTemperature(float temp);
        float getTemperature();
        void inputImu(int x, int y);
        int getState(); //imu 값을 기반으로 cow가 멈춰져 있는 상태인지 판단한다.
                        //return 1 ->stable state 
                        //return 0 ->moving state(unstable)
        

private:  
        int state;
        float temperature =  20.0;
        int imu[3] = { 0, }; //circular queue를 통해 imu x,y 의 합 저장할 것.
        unsigned int count = 0;// imu filter를 위한 count
    
};
#endif