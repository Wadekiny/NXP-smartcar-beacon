# Untitled - By: hxb - 周四 5月 2 2019

import sensor, image, time, pyb,struct
from pyb import UART

#LAB阈值
#(60, 100, 30, 127, 0, 60) 光线不足
#(50, 100, 40, 127, 0, 80) 光线充足
thresholds=[(50, 100, 50, 100, 0, 70)] #灯红光,code=(1<<0)

#(50, 100, -30, 70, -10, 80)光线不足
#(70, 100, -10, 35, -10, 50)光线充足
white_thresholds=[(50,100, -10, 100, -10, 70)]#white

#(20, 70, -10, 10, -10, 20) #光线充足
#(20, 40, -10, 10, 0, 20)
body_thresholds=[(20, 70, -10, 10, -10, 20)]#灯底座阈值

#(40,80, -20, 10, 0, 30)#慢帧率
#(40, 80, -30, 10, 0, 30)#快帧率
lamp_thresolds=[(30,80, -30, 10, 0, 30)]#灯罩阈值

dict={'body':10,'lamp':20}#字典

avoid_methon=dict['lamp']#切换避障方式

###########################################
#基础参数配置
if avoid_methon==dict['body']:
    ob_roi=(100,70,100,150)
    ob_threshold=body_thresholds
    ob_whdif=40                #长与宽的差值
    ob_maxw=90                 #最大长与宽
    ob_maxh=90
else:                        #灯罩
    ob_roi=(0,70,320,160)
    ob_threshold=lamp_thresolds
    ob_whdif=90
    ob_maxw=160
    ob_maxh=130

############################################
#红灯矩形边框筛选条件
light_whdif=60   #长与宽差值
light_maxw=90
light_maxh=90

############################################

#led灯
g_led=pyb.LED(2) # Red LED = 1, Green LED = 2, Blue LED = 3, IR LEDs = 4.
b_led=pyb.LED(3)
#配置摄像头基本参数
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)#320x240
sensor.set_framerate(0<<9|1<<12)#ov7725设置为48MHZ(96/(N+1)) N<<12
sensor.skip_frames(time = 1000)#跳过1000ms的图像

#获取相关摄像头参数
gain=11.765149   #sensor.get_gain_db()

#(-5.368132, -6.02073, -6.296998)
#(-4.52867, -6.02073, -6.157801)
rgb_gain=(-4.52867, -6.02073, -6.157801)#sensor.get_rgb_gain_db()

#配置摄像头感光元件参数
sensor.set_auto_whitebal(False,rgb_gain_db=rgb_gain)#白平衡关闭
sensor.set_auto_gain(False,gain_db=gain)#自动增益关闭
sensor.set_auto_exposure(True)#自动曝光打开
#sensor.set_windowing((0,35,320,170))#设置子分辨率
#sensor.set_brightness(-1)#亮度
#sensor.set_saturation(-1)#饱和度
#sensor.set_contrast(+1)#对比度
clock = time.clock()

#串口初始化
uart = UART(4, 115200)


#############################################
#多个障碍同时存在的筛选条件
centre_x=160   #x坐标更接近的中心值
               #面积更大的更接近车
#############################################

while(True):
    clock.tick()#开始追踪运行时间
    img = sensor.snapshot()#截取图像
    #寻找亮的灯
    min_x=0
    min_y=0
    max_x=0
    max_y=0
    exist=False
    #img.draw_cross(160,120,color=(10,170,30),size=120)
    for blob in img.find_blobs(thresholds,roi=(0,45,img.width(),img.height()-45),pixels_threshold=2, area_threshold=2,merge=True,margin=10):#色块查找
        if  abs(blob.w()-blob.h())<=light_whdif and blob.w()<=light_maxw and blob.h()<=light_maxh:    #筛选条件，减少干扰
            exist=True
            #img.draw_rectangle(blob.rect())
            if blob.x()<min_x or min_x==0:
                min_x=blob.x()
            if blob.y()<min_y or min_y==0:
                min_y=blob.y()
            if blob.x()+blob.w()>max_x:
                max_x=blob.x()+blob.w()
            if blob.y()+blob.h()>max_y:
                max_y=blob.y()+blob.h()
            #print(blob.count())
    #存在红色,在红色中找白色
    if exist:
        red_rect=[min_x,min_y,max_x-min_x,max_y-min_y]
        #B_led.on()
        img.draw_rectangle(red_rect)#画矩形边框
        #img.draw_cross(int(red_rect[0]+red_rect[2]/2),int(red_rect[1]+red_rect[3]/2),
        #                   color=(0,255,0),thickness=3)#画中心十字
        blobs=img.find_blobs(white_thresholds,roi=red_rect,pixel_threshold=1,area_threshold=1,merge=True)
        if blobs:
            for blob in blobs:
                g_led.on()
                #img.draw_rectangle(blob.rect())#画矩形边框
                img.draw_cross(blob.cx(),blob.cy(),color=(0,255,0),thickness=3)#画中心十字
                light_cx=blob.cx()
                light_cy=blob.cy()
                #print("light_cx= ",light_cx," light_cy= ",light_cy)
                break #避免反光
    else:#无红色存在
        g_led.off()
        light_cx=0
        light_cy=0

    #寻找未亮的灯
    max_ob=0  #最大的面积值
    min_xdif=0 #最接近于中心的值
    ob_cx=0
    ob_cy=0
    Ob_exist=False
    img.draw_rectangle(ob_roi)#画矩形边框
    blobs=img.find_blobs(ob_threshold,roi=ob_roi,
                        x_stride=3,y_stride=3,pixels_threshold=130, area_threshold=130,merge=True)#色块查找
    if blobs: #存在符合的颜色色块
        for blob in blobs:
           # print(blob.cx(),blob.cy(),blob.w(),blob.h())
            #筛选出符合条件的，防止干扰项
            if abs(blob.w()-blob.h())<=ob_whdif and blob.w()<=ob_maxw and blob.h()<=ob_maxh:
                Ob_exist=True
                img.draw_rectangle(blob.rect())#画矩形边框
                #img.draw_cross(blob.cx(),blob.cy(),color=(0,0,255),thickness=3)#画中心十字
                #多障碍情况下的优先级筛选
                if  blob.area()>max_ob: #第一优先级为面积
                    max_ob=blob.area()
                    ob_cx=blob.cx()
                    ob_cy=blob.cy()
                elif blob.area()==max_ob:
                    if abs(blob.cx()-centre_x)>min_xdif:
                        min_xdif=abs(blob.cx()-centre_x)
                        ob_cx=blob.cx()
                        ob_cy=blob.cy()
    if Ob_exist==True:
        b_led.on()
        img.draw_cross(ob_cx,ob_cy,color=(0,0,255),thickness=3)#画中心十字
    else:
        b_led.off()
    wdata=struct.pack("<bbHHHHHb",0xEE,0xa2,light_cx,light_cy,ob_cx,ob_cy,0,0xFF)
    uart.write(wdata)#发送数据
    print(clock.fps())#打印帧率
    #print("gain = %f db" % sensor.get_gain_db())
   # print(sensor.get_rgb_gain_db())
