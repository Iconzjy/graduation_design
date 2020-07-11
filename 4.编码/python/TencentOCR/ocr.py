import socket  # 导入 socket 模块
import json
import test

from tencentcloud.common import credential
from tencentcloud.common.profile.client_profile import ClientProfile
from tencentcloud.common.profile.http_profile import HttpProfile
from tencentcloud.common.exception.tencent_cloud_sdk_exception import TencentCloudSDKException
from tencentcloud.ocr.v20181119 import ocr_client, models
import json
import jsonpath

def tencentOCR(src,format):
     try:
          cred = credential.Credential("AKIDqUBfvwgZCan9Ppq93kEVgcHM4QY24Z9U", "IqSTZUQ7nJSPzsxnOo34rQOFsIlx1tcI")
          httpProfile = HttpProfile()
          httpProfile.endpoint = "ocr.tencentcloudapi.com"

          clientProfile = ClientProfile()
          clientProfile.httpProfile = httpProfile
          client = ocr_client.OcrClient(cred, "ap-guangzhou", clientProfile)

          req = models.GeneralBasicOCRRequest()
          params = '{"ImageBase64":"data:image/'+format+';base64,'+src+'"}'
          req.from_json_string(params)

          resp = client.GeneralBasicOCR(req)
          recv = resp.to_json_string()

     except TencentCloudSDKException as err:
          print(err)

     str = json.loads(recv)
     DetectedText = jsonpath.jsonpath(str, "$..DetectedText")
     parseDetect=""
     for msg in DetectedText:
         print(msg)
         parseDetect+=msg+'\n'
     return parseDetect


s = socket.socket()  # 创建 socket 对象
host = socket.gethostname()  # 获取本地主机名
print(host)
port = 7996  # 设置端口
s.bind((host, port))  # 绑定端口i

s.listen(5)  # 等待客户端连接
while True:
    c, addr = s.accept()  # 建立客户端连接
    print('连接地址：', addr)
    msg=""
    while True:
        getmsg = c.recv(1024)
        print(len(getmsg))
        if len(getmsg) >0 :
            msg+=getmsg.decode();
        else:
            break;
    print(len(msg))
    words=tencentOCR(msg,'jpeg')
    c.send(words.encode())
    c.close()  # 关闭连接

