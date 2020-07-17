import sys
import cv2 as cv


def deal(img,n):
    count=0#图片中的人的个数
    file = open("record.txt",'w')
    image = img
    net = n
    h, w = image.shape[:2]


    im_tensor = cv.dnn.blobFromImage(image, size=(300, 300), swapRB=True, crop=False)
    net.setInput(im_tensor)
    cvOut = net.forward()
    i = 0
    confidences = []
    boxes = []
    for detect in cvOut[0,0,:,:]:
        score = detect[2]
        if score > 0.2:
            left = detect[3]*w
            top = detect[4]*h
            right = detect[5]*w
            bottom = detect[6]*h
            width = right-left
            height = bottom-top
            confidences.append(float(score))
            boxes.append([left, top, width, height])

    indices = cv.dnn.NMSBoxes(boxes, confidences, 0.2, 0.4)
    for ii in indices:
        count = count+1
        ii = ii[0]
        box = boxes[ii]
        left = box[0]
        top = box[1]
        width = box[2]
        height = box[3]
        i = i + 1
        cv.putText(image, str(i), (int(left), int(top)), cv.FONT_HERSHEY_COMPLEX, 0.8, (0, 255, 0), 1)
        cv.rectangle(image, (int(left), int(top)), (int(left+width), int(top+height)), (0, 255, 255), 4)
    file.write(str(count))
    file.close()
    return image

if __name__ == "__main__":
    filepath=sys.argv[1]
    storageName=sys.argv[2]

    inference_pb = "sorted_inference_graph.pb"
    graph_txt = "graph.pbtxt"
    net = cv.dnn.readNetFromTensorflow(inference_pb, graph_txt)

    pic = cv.imread(filepath);
    pic1 = deal(pic,net)
    cv.imwrite(storageName,pic)

