#ifndef COW
#define COW
class Cow{
    
public:
        
        void setTemperature(float temp);
        float getTemperature();
        void inputImu(int x, int y);
        int getState(); //imu ���� ������� cow�� ������ �ִ� �������� �Ǵ��Ѵ�.
                        //return 1 ->stable state 
                        //return 0 ->moving state(unstable)
        

private:  
        int state;
        float temperature =  20.0;
        int imu[3] = { 0, }; //circular queue�� ���� imu x,y �� �� ������ ��.
        unsigned int count = 0;// imu filter�� ���� count
    
};
#endif