from pay import AliPay
from urllib.parse import urlparse, parse_qs
import time

def getURL(user):
    """支付请求过程"""
    # 传递参数初始化支付类
    alipay = AliPay(
        appid="2016101800715386",  # 设置签约的appid
        app_notify_url="https://www.zjyxxn.cn/api/asyn",  # 异步支付通知url
        app_private_key_path=u"keys/应用私钥2048.txt",  # 设置应用私钥
        alipay_public_key_path="keys/沙箱公钥.txt",  # 支付宝的公钥，验证支付宝回传消息使用，不是你自己的公钥,
        debug=True,  # 默认False,                                   # 设置是否是沙箱环境，True是沙箱环境
        return_url="https://www.zjyxxn.cn/api/sync"  # 同步支付通知url
    )

    localtime=time.strftime("%Y%m%d%H%M%S", time.localtime())

    # 传递参数执行支付类里的direct_pay方法，返回签名后的支付参数，
    url = alipay.direct_pay(
        subject="测试订单",  # 订单名称
        # 订单号生成，一般是当前时间(精确到秒)+用户ID+随机数
        out_trade_no=localtime+user,  # 订单号
        total_amount=1,  # 支付金额
        return_url="https://www.zjyxxn.cn/mainPage/index.html"  # 支付成功后，跳转url
    )

    # 将前面后的支付参数，拼接到支付网关
    # 注意：下面支付网关是沙箱环境，
    re_url = "https://openapi.alipaydev.com/gateway.do?{data}".format(data=url)
    print(re_url)
    # 最终进行签名后组合成支付宝的url请求
    return re_url


def parseURL(return_url):
    # 将同步支付通知url,传到urlparse
    o = urlparse(return_url)
    # 获取到URL的各种参数
    query = parse_qs(o.query)
    # 定义一个字典来存放，循环获取到的URL参数
    processed_query = {}
    # 将URL参数里的sign字段拿出来
    ali_sign = query.pop("sign")[0]

    # 传递参数初始化支付类
    alipay = AliPay(
        appid="2016101800715386",  # 设置签约的appid
        app_notify_url="https://www.zjyxxn.cn/api/asyn",  # 异步支付通知url
        app_private_key_path=u"keys/应用私钥2048.txt",  # 设置应用私钥
        alipay_public_key_path="keys/沙箱公钥.txt",  # 支付宝的公钥，验证支付宝回传消息使用，不是你自己的公钥,
        debug=True,  # 默认False,                                   # 设置是否是沙箱环境，True是沙箱环境
        return_url="https://www.zjyxxn.cn/api/sync"  # 同步支付通知url
    )

    # 循环出URL里的参数
    for key, value in query.items():
        # 将循环到的参数，以键值对形式追加到processed_query字典
        processed_query[key] = value[0]
    # 将循环组合的参数字典，以及拿出来的sign字段，传进支付类里的verify方法，返回验证合法性，返回布尔值，True为合法，表示支付确实成功了，这就是验证是否是伪造支付成功请求
    print(alipay.verify(processed_query, ali_sign))
    return alipay.verify(processed_query, ali_sign)

# 如果别人伪造支付成功请求，它不知道我们的支付宝公钥，伪造的就无法通过验证，测试可以将支付宝公钥更改一下，在验证就会失败，别忘了改回来
