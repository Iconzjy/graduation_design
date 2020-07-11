#!/usr/bin/env python
# -*- coding:utf8 -*-
from django.shortcuts import render

# Create your views here.
from django.shortcuts import render,HttpResponse
from django.views.decorators.csrf import csrf_exempt,csrf_protect

from pay import AliPay
from setting import ying_yong_si_yao, zhi_fu_bao_gong_yao


def alipaview(request):
    if request.method == "GET":
        processed_dict = {}  # 接收支付宝传递参数
        for key, value in request.GET.items():  # 循环参数
            processed_dict[key] = value  # 将参数添加到字典
        sign = processed_dict.pop('sign', None)  # 单独拿出sign字段

        # 传递参数初始化支付类
        alipay = AliPay(
            appid="2016101800715386",  # 设置签约的appid
            app_notify_url="https://www.zjyxxn.cn/",  # 异步支付通知url
            app_private_key_path=u"keys/应用私钥2048.txt",  # 设置应用私钥
            alipay_public_key_path="keys/沙箱公钥.txt",  # 支付宝的公钥，验证支付宝回传消息使用，不是你自己的公钥,
            debug=True,  # 默认False,                                   # 设置是否是沙箱环境，True是沙箱环境
            return_url="https://www.zjyxxn.cn/"  # 同步支付通知url
        )

        # 验证支付宝返回的合法性
        yan_zhen = alipay.verify(processed_dict, sign)
        if yan_zhen is True:  # 判断如果合法
            out_trade_no = processed_dict.get('out_trade_no', None)  # 商户订单号
            trade_no = processed_dict.get('trade_no', None)  # 支付宝交易号
            buyer_id = processed_dict.get('buyer_id', None)  # 买家支付宝用户号
            trade_status = processed_dict.get('trade_status', None)  # 交易状态
            total_amount = processed_dict.get('total_amount', None)  # 订单金额
            receipt_amount = processed_dict.get('receipt_amount', None)  # 实收金额
            subject = processed_dict.get('subject', None)  # 订单标题
            gmt_payment = processed_dict.get('gmt_payment', None)  # 交易付款时间

            # 数据库操作

            # 向支付宝返回success，告诉他我们已经处理，不然他会不停的通知
            return HttpResponse('success')

    if request.method == "POST":                        # post请求支付宝异步通知
        processed_dict = {}                             # 接收支付宝传递参数
        for key, value in request.POST.items():         # 循环参数
            processed_dict[key] = value                 # 将参数添加到字典
        sign = processed_dict.pop('sign', None)         # 单独拿出sign字段

        # 传递参数初始化支付类
        alipay = AliPay(
            appid="2016101800715386",  # 设置签约的appid
            app_notify_url="https://www.zjyxxn.cn/",  # 异步支付通知url
            app_private_key_path=u"keys/应用私钥2048.txt",  # 设置应用私钥
            alipay_public_key_path="keys/沙箱公钥.txt",  # 支付宝的公钥，验证支付宝回传消息使用，不是你自己的公钥,
            debug=True,  # 默认False,                                   # 设置是否是沙箱环境，True是沙箱环境
            return_url="https://www.zjyxxn.cn/"  # 同步支付通知url
        )

        # 验证支付宝返回的合法性
        yan_zhen = alipay.verify(processed_dict, sign)
        if yan_zhen is True:                                                # 判断如果合法
            out_trade_no = processed_dict.get('out_trade_no', None)         # 商户订单号
            trade_no = processed_dict.get('trade_no', None)                 # 支付宝交易号
            buyer_id = processed_dict.get('buyer_id', None)                 # 买家支付宝用户号
            trade_status = processed_dict.get('trade_status', None)         # 交易状态
            total_amount = processed_dict.get('total_amount', None)         # 订单金额
            receipt_amount = processed_dict.get('receipt_amount', None)     # 实收金额
            subject = processed_dict.get('subject', None)                   # 订单标题
            gmt_payment = processed_dict.get('gmt_payment', None)           # 交易付款时间

            # 数据库操作


            # 向支付宝返回success，告诉他我们已经处理，不然他会不停的通知
            return HttpResponse('success')