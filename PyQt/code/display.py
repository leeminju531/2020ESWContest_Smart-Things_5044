# -*- coding: utf-8 -*-
import sys
import cow

from e_drone.drone import *
from e_drone.protocol import *

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import threading

# from PIL import ImageGrab
import time

import serial

port = 'COM6'
baud = 9600

ser = serial.Serial(port,baud)
data = " "

globalDroneLocation = 0
LocationLock = threading.Lock()  # droneLocation 에 접근하기 위한 mutex

# a = 0
globalDroneTakeoff = False
takeOffLock = threading.Lock()
# b = 0
globalDroneLanding = False
LandingLock = threading.Lock()

drone = Drone()
drone.open("COM9")

def home(current):  # 현재위치 변수로 받기
    global drone
    if current == 1:
        print("1 --> 0")
        # for i in range(5,1,-1):
        #     drone.sendControlPosition(0, 0.3, 0, 0.5, 0, 0)
        #     sleep(1)
        drone.sendControlPosition(0, 0.3, 0, 0.5, 0, 0)
        sleep(5)
        

    elif current == 2:
        print("2 --> 0")
        drone.sendControlPosition(-1.0, 0.3, 0, 0.5, 0, 0)
        for i in range(5, 0, -1):
            print("{0}".format(i))
            sleep(1)

    

    elif current == 3:
        print("3 --> 0")
        drone.sendControlPosition(-1.0, 1.3, 0, 0.5, 0, 0)
        for i in range(5, 0, -1):
            print("{0}".format(i))
            sleep(1)

        

    elif current == 4:
        print("4 --> 0")
        drone.sendControlPosition(0, 1.3, 0, 0.5, 0, 0)
        for i in range(5, 0, -1):
            print("{0}".format(i))
            sleep(1)

        

    elif current == 5:
        print("5 --> 0")
        drone.sendControlPosition(-0.5, 0.8, 0, 0.5, 0, 0)
        for i in range(5, 0, -1):
            print("{0}".format(i))
            sleep(1)

        


def moveDrone(current, future):  # (현재위치, 가야할 위치)
    global drone
    if current == 0:
        if future == 1:
            print("0 --> 1")
            drone.sendControlPosition(0, -0.3, 0, 0.5, 0, 0)
            sleep(3)
            return future

        elif future == 2:
            print("0 --> 2")
            drone.sendControlPosition(1, -0.3, 0, 0.5, 0, 0)
            sleep(5)
            return future


        elif future == 3:
            print("0 --> 3")
            drone.sendControlPosition(1.0, -1.3, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

        elif future == 4:
            print("0 --> 4")
            
            drone.sendControlPosition(0, -1.3, 0, 0.5, 0, 0)
            sleep(5)
                

            return future

        elif future == 5:
            print("0 --> 5")
            
            drone.sendControlPosition(0.5, -0.8, 0, 0.5, 0, 0)
            sleep(5)
                

            return future

    elif current == 1:
        if future == 1:
            print("1 --> 1")
            drone.sendControlWhile(0, 0, 0, 0, 3000)
            sleep(3)
            return future

        elif future == 2:
            print("1 --> 2")
            drone.sendControlPosition(1.0, 0, 0, 0.5, 0, 0)
            sleep(5)
  
            return future

        elif future == 3:
            print("1 --> 3")
            drone.sendControlPosition(1.0, -1.0, 0, 0.5, 0, 0)
            sleep(5)
      
            return future

        elif future == 4:
            print("1 --> 4")
            drone.sendControlPosition(0, -1.0, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

        elif future == 5:
            print("1 --> 5")
            drone.sendControlPosition(0.5, -0.5, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

    elif current == 2:
        if future == 1:
            print("2 --> 1")
            drone.sendControlPosition(-1.0, 0, 0, 0.5, 0, 0)
            sleep(5)
                
            return future
        elif future == 2:
            print("2 --> 2")
            drone.sendControlWhile(0, 0, 0, 0, 3000)
            sleep(3)
            return future

        elif future == 3:
            print("2 --> 3")
            drone.sendControlPosition(0, -1.0, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

        elif future == 4:
            print("2 --> 4")
            drone.sendControlPosition(-1.0, -1.0, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

        elif future == 5:
            print("2 --> 5")
            drone.sendControlPosition(-0.5, -0.5, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

    elif current == 3:
        if future == 1:
            print("3 --> 1")
            drone.sendControlPosition(-1.0, 1.0, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

        elif future == 2:
            print("3 --> 2")
            drone.sendControlPosition(0, 1.0, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

        elif future == 3:
            print("3 --> 3")
            drone.sendControlWhile(0, 0, 0, 0, 3000)
            sleep(5)
                
            return future

        elif future == 4:
            print("3 --> 4")
            drone.sendControlPosition(-1.0, 0, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

        elif future == 5:
            print("3 --> 5")
            drone.sendControlPosition(-0.5, 0.5, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

    elif current == 4:
        if future == 1:
            print("4 --> 1")
            drone.sendControlPosition(0, 1.0, 0, 0.5, 0, 0)
            sleep(5)
                
            return future
        elif future == 2:
            print("4 --> 2")
            drone.sendControlPosition(1.0, 1.0, 0, 0.5, 0, 0)
            sleep(5)
               
            return future

        elif future == 3:
            print("4 --> 3")
            drone.sendControlPosition(1.0, 0, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

        elif future == 4:
            print("4 --> 4")
            drone.sendControlWhile(0, 0, 0, 0, 3000)
            sleep(5)
                
            return future

        elif future == 5:
            print("4 --> 5")
            drone.sendControlPosition(0.5, 0.5, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

    elif current == 5:
        if future == 1:
            print("5 --> 1")
            drone.sendControlPosition(-0.5, 0.5, 0, 0.5, 0, 0)
            sleep(5)
                
            return future
        elif future == 2:
            print("5 --> 2")
            drone.sendControlPosition(0.5, 0.5, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

        elif future == 3:
            print("5 --> 3")
            drone.sendControlPosition(0.5, -0.5, 0, 0.5, 0, 0)
            sleep(5)
                
            return future

        elif future == 4:
            print("5 --> 4")
            drone.sendControlPosition(-0.5, -0.5, 0, 0.5, 0, 0)
            sleep(5)
            
            return future

        elif future == 5:
            print("5 --> 5")
            drone.sendControlWhile(0, 0, 0, 0, 3000)
            sleep(5)
               
            return future


def DroneAct():
    
    global drone
    currentDroneLocation = 0
    global globalDroneTakeoff,globalDroneLanding,globalDroneLocation
    droneCanMove = False

    while (1):
        LocationLock.acquire()
        futureDroneLocation = globalDroneLocation  # drone 이 가야할 위치를 drone 변수에 저장
        LocationLock.release()
        
        takeOffLock.acquire()
        droneTakeoff = globalDroneTakeoff
        takeOffLock.release()

        LandingLock.acquire()
        droneLanding = globalDroneLanding
        LandingLock.release()


        if droneTakeoff == True:
            takeOffLock.acquire()
            globalDroneTakeoff = False
            takeOffLock.release()
            print('드론 이륙')
            drone.sendTakeOff()
            sleep(5)

            droneCanMove = True

        if droneLanding == True:
            LandingLock.acquire()
            globalDroneLanding = False
            LandingLock.release()
            print('드론 착륙')
            # home(currentDroneLocation)

            drone.sendLanding()
            sleep(5)
            drone.close()
            

        if droneCanMove == True:
            if futureDroneLocation != 0:
                currentDroneLocation = moveDrone(currentDroneLocation,futureDroneLocation)
        


class MySignal(QThread):
    signal = pyqtSignal()

    def run(self):
        global data
        while 1:
            time.sleep(1)
            if ser.readable:
                res = ser.readline()
                code = res.decode()
                data = str(code)
                print(data)
                # print(len(data))
                # print(type(data))
                self.signal.emit()


# 드론위치정보, cow1위치,온도, 2위치,온도,3위치,온도


class Myapp(QDialog, cow.Ui_Dialog):
    def __init__(self):
        QDialog.__init__(self, None)
        self.setupUi(self)
        self.initUI()

        self.worker = MySignal()
        self.worker.signal.connect(self.renewDisplay)
        self.worker.start()
        self.button = QPushButton("", self)
        self.button.setStyleSheet("border-image: url(:/cow/bumped.jpg)")
        self.button.clicked.connect(self.click)

    def click(self):
        self.button.hide()
        # print("pressed")

    def waterStart_50(self):

        data = '1'
        ser.write(data.encode())
        print('수위 50 % 충전 시도')
        
    def waterStart_70(self):
        data = '2'
        ser.write(data.encode())
        print('수위 70 % 충전 시도')
        
  
    def waterStart_100(self):
        data = '3'
        ser.write(data.encode())
        print('수위 100 % 충전 시도')
        

    def droneTakeoff(self):
        global globalDroneTakeoff
        takeOffLock.acquire()
        globalDroneTakeoff = True
        takeOffLock.release()
        

    def droneLanding(self):
        global globalDroneLanding
        LandingLock.acquire()
        globalDroneLanding = True
        LandingLock.release()

    def fenceBump(self, x, y):
        # print(x, y, '좌표출력')
        if x == 1:
            if y == 5:
                self.button.show()
                self.button.setGeometry(1290, 90, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 4:
                self.button.show()
                self.button.setGeometry(1030, 90, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 3:
                self.button.show()
                self.button.setGeometry(780, 90, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 2:
                self.button.show()
                self.button.setGeometry(600, 90, 80, 80)
                self.button.clicked.connect(self.click)
            if y == 1:
                self.button.show()
                self.button.setGeometry(220, 90, 80, 80)
                self.button.clicked.connect(self.click)
            # if y == 1:
            #     self.button.show()
            #     self.button.setGeometry(1380, 100, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 2:
            #     self.button.show()
            #     self.button.setGeometry(1380, 250, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 3:
            #     self.button.show()
            #     self.button.setGeometry(1380, 400, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 4:
            #     self.button.show()
            #     self.button.setGeometry(1380, 550, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 5:
            #     self.button.setGeometry(1380, 700, 80, 80)
            #     self.button.show()
            #     self.button.clicked.connect(self.click)

        elif x == 2:
            if y == 1:
                self.button.show()
                self.button.setGeometry(1430, 150, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 2:
                self.button.show()
                self.button.setGeometry(1430, 300, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 3:
                self.button.show()
                self.button.setGeometry(1430, 450, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 4:
                self.button.show()
                self.button.setGeometry(1430, 600, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 5:
                self.button.setGeometry(1430, 750, 80, 80)
                self.button.show()
                self.button.clicked.connect(self.click)
            # if y == 1:
            #     self.button.setGeometry(1240, 810, 80, 80)
            #     self.button.show()
            #     self.button.clicked.connect(self.click)  # 이를 2번 호출할 시에 문제가 생김

            
            # if y == 2:
            #     self.button.show()
            #     self.button.setGeometry(980, 810, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 3:
            #     self.button.show()
            #     self.button.setGeometry(730, 810, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 4:
            #     self.button.show()
            #     self.button.setGeometry(550, 810, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 5:
            #     self.button.show()
            #     self.button.setGeometry(170, 810, 80, 80)
            #     self.button.clicked.connect(self.click)



        elif x == 3:
            if y == 1:
                self.button.setGeometry(1290, 860, 80, 80)
                self.button.show()
                self.button.clicked.connect(self.click)  # 이를 2번 호출할 시에 문제가 생김

            
            if y == 2:
                self.button.show()
                self.button.setGeometry(1030, 860, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 3:
                self.button.show()
                self.button.setGeometry(780, 860, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 4:
                self.button.show()
                self.button.setGeometry(600, 860, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 5:
                self.button.show()
                self.button.setGeometry(220, 860, 80, 80)
                self.button.clicked.connect(self.click)
            
            # if y == 1:
            #     self.button.show()
            #     self.button.setGeometry(60, 690, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 2:
            #     self.button.show()
            #     self.button.setGeometry(60, 540, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 3:
            #     self.button.show()
            #     self.button.setGeometry(60, 400, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 4:
            #     self.button.show()
            #     self.button.setGeometry(60, 250, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 5:
            #     self.button.show()
            #     self.button.setGeometry(60, 100, 80, 80)
            #     self.button.clicked.connect(self.click)

        elif x == 4:
            if y == 1:
                self.button.show()
                self.button.setGeometry(110, 750, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 2:
                self.button.show()
                self.button.setGeometry(110, 600, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 3:
                self.button.show()
                self.button.setGeometry(110, 450, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 4:
                self.button.show()
                self.button.setGeometry(110, 300, 80, 80)
                self.button.clicked.connect(self.click)

            if y == 5:
                self.button.show()
                self.button.setGeometry(110, 150, 80, 80)
                self.button.clicked.connect(self.click)
            # if y == 1:
            #     self.button.show()
            #     self.button.setGeometry(1240, 40, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 2:
            #     self.button.show()
            #     self.button.setGeometry(980, 40, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 3:
            #     self.button.show()
            #     self.button.setGeometry(730, 40, 80, 80)
            #     self.button.clicked.connect(self.click)

            # if y == 4:
            #     self.button.show()
            #     self.button.setGeometry(550, 40, 80, 80)
            #     self.button.clicked.connect(self.click)
            # if y == 5:
            #     self.button.show()
            #     self.button.setGeometry(170, 40, 80, 80)
            #     self.button.clicked.connect(self.click)
            

    def renewDisplay(self):
        global globalDroneLocation

        if len(data) == 44:  # data 길이가 21일 때, 버퍼가 밀리지않고 가져왔다고 판단가능( 처음 시작시, 오류 가능)

            LocationLock.acquire()
            globalDroneLocation = int(data[0])# drone 이 가야할 위치를 drone 변수에 저장
            print('globalDroneLocation :',globalDroneLocation)
            LocationLock.release()

            self.cow1Location = int(data[1])
            if self.cow1Location != 0:
                if self.cow1Location == 1:
                    self.label_18.move(1100, 260)
                elif self.cow1Location == 2:
                    self.label_18.move(1100, 570)
                elif self.cow1Location == 3:
                    self.label_18.move(150, 560)
                elif self.cow1Location == 4:
                    self.label_18.move(140, 140)
                elif self.cow1Location == 5:
                    self.label_18.move(630, 370)
            self.cow1Temperature = float(data[2:7])
            if self.cow1Temperature != 0.0:
                self.lcdNumber.display(self.cow1Temperature)

            self.cow2Location = int(data[7])
            if self.cow2Location != 0:
                if self.cow2Location == 1:
                    self.label_19.move(1100, 140)
                elif self.cow2Location == 2:
                    self.label_19.move(1100, 660)
                elif self.cow2Location == 3:
                    self.label_19.move(140, 680)
                elif self.cow2Location == 4:
                    self.label_19.move(150, 270)
                elif self.cow2Location == 5:
                    self.label_19.move(640, 490)

            self.cow2Temperature = float(data[8:13])
            if self.cow2Temperature != 0.0:
                self.lcdNumber_2.display(self.cow2Temperature)

            self.cow3Location = int(data[13])
            if self.cow3Location != 0:
                if self.cow3Location == 1:
                    self.label_20.move(1240, 250)
                elif self.cow3Location == 2:
                    self.label_20.move(1230, 670)
                elif self.cow3Location == 3:
                    self.label_20.move(280, 680)
                elif self.cow3Location == 4:
                    self.label_20.move(270, 260)
                elif self.cow3Location == 5:
                    self.label_20.move(760, 490)

            self.cow3Temperature = float(data[14:19])
            if self.cow3Temperature != 0.0:
                self.lcdNumber_3.display(self.cow3Temperature)

            self.waterPercent = int(data[19:22])
            if self.waterPercent != -1:
                self.progressBar.setValue(self.waterPercent)
            
            self.fence1 = [data[22], data[23], data[24], data[25], data[26]]  # int(data[22:27])
            self.fence2 = [data[27], data[28], data[29], data[30], data[31]]  # int(data[27:32])
            self.fence3 = [data[32], data[33], data[34], data[35], data[36]]  # int(data[32:37])
            self.fence4 = [data[37], data[38], data[39], data[40], data[41]]  # int(data[37:42])
            self.Fence = [self.fence1, self.fence2, self.fence3, self.fence4]
            

            i = 0
            j = 0
            

            for fence in self.Fence:
                i = i + 1
                j = 0
                # print('fence : ',fence)
                for fen in fence:
                    j = j + 1
                    # print('fen:',fen, type(fen))
                    if int(fen) == 1:
                        print('i :', i, 'j :', j)
                        Myapp.fenceBump(self, i, j)

    def initUI(self):
        self.setWindowTitle('방목 현황')
        self.progressBar.setValue(0)  # 블루투스(수위센서 값)받아오면 % 출력
        self.lcdNumber.display(0)
        self.lcdNumber_2.display(0)
        self.lcdNumber_3.display(0)
        self.water.clicked.connect(self.waterStart_50)
        self.water_2.clicked.connect(self.waterStart_70)
        self.water_3.clicked.connect(self.waterStart_100)
        self.takeOff.clicked.connect(self.droneTakeoff)
        self.landing.clicked.connect(self.droneLanding)
        # 블루투스(소 온도)받아오면 출력


if __name__ == '__main__':
    app = QApplication(sys.argv)
    main_dialog = Myapp()
    main_dialog.show()
    droneThread = threading.Thread(target=DroneAct)
    droneThread.daemon = True # 메인루프 종료시 쓰레드 같이 종료
    droneThread.start()

    app.exec_()  # 루프문,이벤트 처리

