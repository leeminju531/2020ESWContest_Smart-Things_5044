# SmartThings-Bluetooth_ibeacon

## 임베디드 소프트웨어 경진대회 제 18회 SmartThings 부문 - 날아야하니 팀

### 시연 동영상
https://youtu.be/fELvObNoA9s

### 프로젝트 개요
최근 국내 축산물 자급률이 감소하며, 6차 산업형 축산이 주목을 받고 있다. 6차 산업형 축산은 숲속 초지나 쓰지 않는 산지를 활용해 가축(방목)을 키우는 자연 친화적인 축산의 한 형태이다. 이 산업은 관광과도 접목시킬 수 있다는 장점이 있는데 국내에서는 활성화가 어려운 현실이다. 주된 이유는 초지 조성 및 방목 관리비용 부담이 크다는 것이다. 또한 부실한 관리로 인한 환경오염 및 산림훼손의 가능성 또한 존재하므로 업계에서는 쉽게 추진하지 못하고 있다. 뿐만 아니라 축산업 종사자들의 고령화 문제로 인하여 ICT융합기술이 접목된 축산분야 개발이 절실히 요구되어지고 있는데, 기존 ICT기술은 축사 내에서 동물을 모니터링하는 시스템만 개발되어 왔다. 따라서 우리는 이러한 문제를 ICT기술이 융합된 smart-방목농장 구현을 통해 해결하고, 더 나아가 국내 산지생태 축산의 활성화에 이바지하려 한다. 

#### 개발환경
-cows, ScanningBoard 폴더 -> 아두이노 MEGA 보드.(블루투스 hm-10의 SCANNING 데이터를 저장하기 위해서, Uno 보드는 메모리가 부족함)
-TOWER1,2,3,4,5 폴더 -> 아두이노 Uno 보드. 모든 아두이노 보드는 C++을 사용한 공식 아두이노 IDE1.8.13 버전을 사용
-	코드론2 -> Python 언어를 사용하였으며, 제공되는 코드론 API를 사용
-	PYQT5 -> 파이썬 언어를 사용하여 미리 제작한 디스플레이를 갱신할 수 있도록 구성
(파이썬 언어는 3.x 버전을 통해 구성)

#### 파일 구성 안내
1)소 - Cows 파일


2)Tower1/2/3/4 - Tower1234_with_fence 파일


3)Tower5 - Tower5_with_waterSensor 파일


4)Tower5 와 시리얼 연결된 모터쉴드보드 - Tower5FriendMotor 파일


5)Scanning 전용 보드 - ScanningBoard 파일
	
	
6)PYQT 디스플레이 & 드론


PyQt 폴더 안에는 이미지파일 / code / __pycache__ 파일이 존재하며, 이미지파일은 PYQT 디스플레이를 구성하기 위해 사용한 이미지파일들이 존재한다.
Code 파일에는 cow.py / cow.qrc / cow.iu / cow_rc.py / display.py 가 존재한다. 앞의 4가지 파일은 미리 제작한 PYQT GUI를 제작하였으며, display.py 파일에서 이러한 파일을 import하여 드론과 같이 동작한다.


* AT_Queue 폴더 / cowlib 폴더 : 아두이노에서 사용되는 사용자 정의 라이브러리로서, 명령어를 저장하는 큐(AT_Queue) 와 cow의 정보를 저장하는 클래스가 선언 되어있는 파일이다. 이 두파일은 아두이노 라이브러리에 추가해야 한다. ( 일반적으로, 아두이노 라이브러리 경로는 C:/Program Files(x86) / Arduino / libraries 에 위치함)


* MsTimer2 라이브러리 : 아두이노에서 타이머 인터럽트를 사용하기 위한 라이브러리이다. 아두이노 공식 사이트에서 제공되는 라이브러리이지만, MEGA보드에서는 MsTimer2의 지원하고 있지 않아 라이브러리 헤더파일을 수정하여 추가해주었다.

### 알고리즘
1. cows
![cow](https://user-images.githubusercontent.com/70446214/95673275-1c491c80-0be2-11eb-983a-d34f9e3f1ab1.png)
2. Tower1,2,3,4
![tower1234](https://user-images.githubusercontent.com/70446214/95673307-50bcd880-0be2-11eb-8571-8bd61dd1e45c.png)
3. Tower5
![tower5](https://user-images.githubusercontent.com/70446214/95673309-51556f00-0be2-11eb-8799-76f78fbde841.png)
4. ScanningBoard
![ScanningBoard](https://user-images.githubusercontent.com/70446214/95673312-52869c00-0be2-11eb-8ab4-226a23b8ec9c.png)
5. PYQT & DRONE
![pythonprocess](https://user-images.githubusercontent.com/70446214/95673313-53b7c900-0be2-11eb-8439-1907f8dbceaf.png)
