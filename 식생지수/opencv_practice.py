import cv2
from PIL import ImageGrab

while 1:
    img = ImageGrab.grab(bbox=(0, 185, 1230, 660))  # 드론의 rgb카메라 영상을 노트북화면에 그대로 띄워 1초마다 캡쳐한다.
    img.save('drone.png')
    img_color = cv2.imread('D:\pyqt5_study\Form\_codeExample\drone.png')
    img_color = cv2.resize(img_color, dsize=(530, 400), interpolation=cv2.INTER_AREA)
    cv2.imshow('original', img_color)
    cv2.moveWindow('original', 1000, 0)
    img_hsv = cv2.cvtColor(img_color, cv2.COLOR_BGR2HSV)  # 마스크 처리에 용의하게 hsv (색상,채도,명도) 색공간으로 변환 // 연두색[68,15,199] //노란색 [19,122,213]
    # 파란소hsv[118,169,254] // 빨간소[6,199,231] // 울타리105,37,79] //흰색[174,19,219] [0,21,209] //갈색[11,150,161] [2,158,189] //바깥[11,158,79] // 검정소[111,72,35]이지만 어차피 검정색이므로 마스크처리 안해도됨
    lower_gray = (5, 100, 50)
    upper_gray = (21, 200, 90)
    lower_white = (80, 0, 200)
    upper_white = (174 + 10, 50, 255) # 범위내의 픽셀들은 흰색, 나머지 검은색
    gray_mask = cv2.inRange(img_hsv, lower_gray, upper_gray)
    gray_mask2 = cv2.bitwise_and(img_color, img_color, mask=gray_mask)
    white_mask = cv2.inRange(img_hsv, lower_white, upper_white)
    white_mask2 = cv2.bitwise_and(img_color, img_color, mask=white_mask)
    img_result = img_color - white_mask2 - gray_mask2

    height, width = img_result.shape[:2]
    grass_result = cv2.cvtColor(img_result, cv2.COLOR_BGR2GRAY)  #EXG 계산 결과가 저장될 이미지, 그레이스케일로 변환한 뒤 EXG를 계산한다.
    total_area = 0
    grass_area = 0
    percent = 0
    for y in range(0, height):
        for x in range(0, width):
            b = img_result.item(y, x, 0)
            g = img_result.item(y, x, 1)
            r = img_result.item(y, x, 2)
            if b < 10 and g < 10 and r < 10:  # 애초에 검정색인 픽셀은 마스크 처리를 거쳐 관심 밖의 영역이라 판단한다.
                EXR = 0
            else:
                EXR = 1400 * (2 * g / (b + g + r) - r / (b + g + r) - b / (b + g + r)) #EXG(초과녹색식생지수) 계산식, 0에 가까울수록 비식생영역이고 255에 가까울수록 식생영역
                if EXR <= 0:
                    EXR = 0
                elif EXR >= 255:
                    EXR = 255
                    total_area = total_area + 1
                    grass_area = grass_area + 1
                else:
                    total_area = total_area + 1  # 0<EXR<255 이면 무조건 관심영역
                    if EXR >= 60:                # 60<EXR 이면 그 중에서도 식생영역이라 판단
                        grass_area = grass_area + 1
            grass_result.itemset(y, x, EXR)
    if total_area != 0:
        percent = round(grass_area / total_area * 100, 1)
    print("total_area={}, grass_area={}, grass_percent={}%".format(total_area,grass_area,percent))
    cv2.imshow('After_Calculating', grass_result)
    cv2.moveWindow('After_Calculating', 1000, 400)
    cv2.waitKey(500)
cv2.destroyAllWindows()




