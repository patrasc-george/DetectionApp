import datetime
import tkinter
from enum import Enum
from tkinter import *
from tkinter import filedialog, ttk

import numpy as np
from PIL import Image, ImageTk
import cv2 as cv
import detection


# HELPERS ==============================================================================================================

class ViewType(Enum):
    OFF = 0
    IMAGE = 1
    CAMERA = 2


class Singleton(type):  # I got this one from the internet, IDK don't ask
    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]


class Window(metaclass=Singleton):

    def __init__(self):
        self.cap = cv.VideoCapture()
        self.view_state = ViewType.OFF
        self.image = None
        self.action_just_happened = True  # used so we only update the view on camera
        self.current_detector_name = ''  # see detectors list in detection.py

    # BUTTON EVENTS ========================================================================================================
    def screenshot(self):
        image = Image.fromarray(self.image[:, :, ::-1])
        time = str(datetime.datetime.now().today()).replace(':', "_") + '.jpg'
        f_types = [('PNG image', '*.png'), ('JPG image', '*.jpg'), ('JPEG image', '*.jpeg')]
        filename = filedialog.asksaveasfile(filetypes=f_types, initialfile=time)
        image.save(filename)

    def upload_file(self, ):
        self.action_just_happened = True
        self.view_state = ViewType.IMAGE
        f_types = [('Image Files', '*.png *.jpg *.jpeg')]
        filename = filedialog.askopenfilename(filetypes=f_types)
        image = cv.imread(filename)
        if image is not None:
            self.image = image
            self.view_state = ViewType.IMAGE

    def toggle_camera(self):
        self.action_just_happened = True
        if self.view_state != ViewType.CAMERA:
            self.cap.open(0)
            if not self.cap.isOpened():
                print("Unable to read camera feed")
                self.cap.release()
                pass
            self.view_state = ViewType.CAMERA
        else:
            self.cap.release()
            self.view_state = ViewType.OFF


# GUI ==================================================================================================================
w = Window()
root = Tk()
root.title("Object Detection")
root.minsize(640, 580)
root.configure(bg='#fff')

frame_label = LabelFrame(root)
frame_label.grid(row=1, column=1, columnspan=4)
image_box = Label(frame_label)

image_box.pack()

camera_button = Button(root, text="Toggle Camera", command=w.toggle_camera)
screenshot_button = Button(root, text='Screenshot', command=w.screenshot)
upload_button = Button(root, text='Upload Image', command=w.upload_file)

w.current_detector_name = tkinter.StringVar()
detector_dropdown = ttk.Combobox(root,
                                 textvariable=w.current_detector_name,
                                 state='readonly',
                                 values=[name for name in detection.nets.keys()])
detector_dropdown.bind("<<ComboboxSelected>>", lambda event: setattr(w, 'action_just_happened', True))
detector_dropdown.set(detector_dropdown['values'][0])

camera_button.grid(row=2, column=1)
screenshot_button.grid(row=2, column=2)
upload_button.grid(row=2, column=3)
detector_dropdown.grid(row=2, column=4)

# APP LOOP =============================================================================================================
while True:
    if w.view_state != ViewType.CAMERA and not w.action_just_happened:
        root.update()
        continue
    match w.view_state:
        case ViewType.CAMERA:
            w.image = w.cap.read()[1]
            w.image = cv.flip(w.image, 1)
            w.image = detection.detect(w.image, w.current_detector_name.get())
            img1 = cv.cvtColor(w.image, cv.COLOR_BGR2RGB)
            img = ImageTk.PhotoImage(Image.fromarray(img1))
            image_box['image'] = img
        case ViewType.IMAGE:
            w.image = detection.detect(w.image, w.current_detector_name.get())  # don't show fps on image
            img1 = cv.cvtColor(w.image, cv.COLOR_BGR2RGB)
            img = ImageTk.PhotoImage(Image.fromarray(img1))
            image_box['image'] = img
        case ViewType.OFF:
            emp = np.zeros((480, 640), "uint8")  # empty image
            emp.fill(255)
            w.image = cv.cvtColor(emp, cv.COLOR_GRAY2RGB)
            img = ImageTk.PhotoImage(Image.fromarray(w.image))
            image_box['image'] = img
    w.action_just_happened = False
    root.update()
