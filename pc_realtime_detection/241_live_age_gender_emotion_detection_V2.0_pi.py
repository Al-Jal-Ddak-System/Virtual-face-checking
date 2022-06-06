# https://youtu.be/JmvmUWIP2v8
"""
Live prediction of emotion, age, and gender using pre-trained models. 
Uses haar Cascades classifier to detect face..
then, uses pre-trained models for emotion, gender, and age to predict them from 
live video feed. 

"""
import ctypes
from keras.models import load_model
from time import sleep
from tensorflow.keras.utils import img_to_array # from keras.preprocessing.image import img_to_array
from keras.preprocessing import image
import cv2
import numpy as np

# ====================== socket
from time import sleep
from socket import *
import sys

args = sys.argv
# print(args)

csock = socket(AF_INET, SOCK_STREAM)
# csock.connect(("192.168.0.14", 8888))
csock.connect((args[1], int(args[2])))
# ====================== socket


face_classifier=cv2.CascadeClassifier('./haarcascade/haarcascade_frontalface_default.xml')
emotion_model = load_model('emotion_detection_model_100epochs.h5')

class_labels=['Angry','Disgust', 'Fear', 'Happy','Neutral','Sad','Surprise']

cap=cv2.VideoCapture(0)

while True:
    ret,frame=cap.read()
    labels=[]
    
    gray=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    faces=face_classifier.detectMultiScale(gray,1.3,5)

    for (x,y,w,h) in faces:
        cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),2)
        roi_gray=gray[y:y+h,x:x+w]
        roi_gray=cv2.resize(roi_gray,(48,48),interpolation=cv2.INTER_AREA)

        #Get image ready for prediction
        roi=roi_gray.astype('float')/255.0  #Scale
        roi=img_to_array(roi)
        roi=np.expand_dims(roi,axis=0)  #Expand dims to get it ready for prediction (1, 48, 48, 1)

        preds=emotion_model.predict(roi)[0]  #Yields one hot encoded result for 7 classes
        label=class_labels[preds.argmax()]  #Find the label
        label_position=(x,y)
        cv2.putText(frame,label,label_position,cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,0),2)

        # ====================== socket
        line = preds.argmax()
        print(label, line)
        # C_sock = ctypes.CDLL('./client.so')
        # ad = C_sock.main()
        # C_sock.data(int(line),ad)
        # ccc = csock.recv(1024)
        #print(ccc)
        #csock.sendall(line.encode())
        # ====================== socket
   
   
    cv2.imshow('Emotion Detector', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()