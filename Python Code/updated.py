import os
import cv2
import serial
import supervision as sv
from ultralytics import YOLOv10

# Set up the YOLO model
model = YOLOv10(f'/home/mighty/Code/python/Object_Detection/best.pt')

# Set up the serial connection to the ESP32
serial_port = '/dev/ttyUSB0'  # Update this to your ESP32 serial port
baud_rate = 9600
ser = serial.Serial(serial_port, baud_rate, timeout=1)

bounding_box_annotator = sv.BoundingBoxAnnotator()
label_annotator = sv.LabelAnnotator()

# Open the camera
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Unable to load camera feed")
    exit()

# Main loop for object detection
while True:
    ret, frame = cap.read()

    if not ret:
        continue

    # Perform object detection
    results = model(frame)[0]
    detections = sv.Detections.from_ultralytics(results)

    # Annotate the detection
    annotated_image = bounding_box_annotator.annotate(scene=frame, detections=detections)
    annotated_image = label_annotator.annotate(scene=annotated_image, detections=detections)

    # Show the camera feed with annotations
    cv2.imshow('Webcam', annotated_image)

    # If any object is detected, send a signal to the ESP32 to move forward
    if len(detections) > 0:
        ser.write(b'F')  # 'F' for moving forward
    else:
        ser.write(b'R')  # 'R' for rotating/searching for garbage

    k = cv2.waitKey(1)

    if k % 256 == 27:  # Escape key pressed
        print("Escape hit, closing...")
        break

cap.release()
cv2.destroyAllWindows()
