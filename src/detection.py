import cv2
import datetime

# INITIALIZATION =======================================================================================================

"""
    Detection matrix components:
    [1]: Class ID
    [2]: Probability (0-1)
    [3]: Bounding Box X coord
    [4]: Bounding Box Y coord
    [5]: Bounding Box Width
    [6]: Bounding Box Height
"""

detectors = {
    'MobileNet v3 large': {
        'weights': "models/MobileNet v3 large/frozen_inference_graph.pb",
        'model': "models/MobileNet v3 large/ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt",
        'classes': "models/MobileNet v3 large/coco_names.txt",
        'framework': 'tensorflow'
    },
    2: {
        # pune modelul aici, George
    }
}

nets = {}
for name in detectors:
    if isinstance(name, str):
        nets[name] = cv2.dnn.readNet(detectors[name]['model'], detectors[name]['weights'], detectors[name]['framework'])


# DETECTION ============================================================================================================
def detect(frame: cv2.Mat, net_name, show_fps: bool = True):
    with open(detectors[net_name]['classes'], "r") as f:
        class_names = f.read().strip().split("\n")

    # starter time to computer the fps
    start = datetime.datetime.now()
    h = frame.shape[0]
    w = frame.shape[1]

    # create a blob from the frame
    blob = cv2.dnn.blobFromImage(
        image=frame,
        scalefactor=1.0/127.5,
        size=(320, 320),
        mean=[127.5, 127.5, 127.5]
    )
    # pass the blob through the network and get the output predictions
    nets[net_name].setInput(blob)
    output = nets[net_name].forward()  # shape: (1, 1, 100, 7)

    # loop over the number of detected objects
    for detection in output[0, 0, :, :]:  # output[0, 0, :, :] has a shape of: (100, 7)
        # the confidence of the model regarding the detected object
        probability = detection[2]
        # if the confidence of the model is lower than 50%,
        # we do nothing
        if probability < 0.5:
            continue

        # extract the ID of the detected object to get
        class_id = int(detection[1])
        label = class_names[class_id - 1].upper()
        color = (0, 255, 0)
        blue, green, red = int(color[0]), int(color[1]), int(color[2])
        # perform element-wise multiplication to get
        # the (x, y) coordinates of the bounding box
        box = [int(a * b) for a, b in zip(detection[3:7], [w, h, w, h])]
        box = tuple(box)
        # draw the bounding box of the object
        cv2.rectangle(frame, box[:2], box[2:], (blue, green, red), thickness=2)

        # draw the name of the predicted object along with the probability
        text = f"{label} {probability * 100:.2f}%"
        cv2.putText(frame, text, (box[0], box[1] - 10),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 255, 0), 2)

    # end time to compute the fps
    end = datetime.datetime.now()
    # calculate the frame per second and draw it on the frame
    if show_fps:
        fps = f"FPS: {1 / (end - start).total_seconds():.2f}"
        cv2.putText(frame, fps, (5, 25),
                    cv2.FONT_HERSHEY_SIMPLEX, .75, (0, 0, 255), 2)
