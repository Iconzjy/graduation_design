import socket  # 导入 socket 模块
import json
import test

s = socket.socket()  # 创建 socket 对象
host = socket.gethostname()  # 获取本地主机名
print(host)
port = 7998  # 设置端口
s.bind((host, port))  # 绑定端口



s.listen(5)  # 等待客户端连接
while True:
    c, addr = s.accept()  # 建立客户端连接
    print('连接地址：', addr)
    getmsg = c.recv(1024)
    print(getmsg)
    j = json.loads(getmsg.decode(encoding="utf-8"))
    if j["1"]=="1":#获得支付宝支付链接
        c.send(test.getURL(str(j["2"])).encode())
    elif j["1"]=="2":
        c.send(str(test.parseURL(str(j["2"]))).encode())
    c.close()  # 关闭连接
