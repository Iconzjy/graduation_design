from django.conf.urls import url
from django.contrib import admin
from view import alipaview

urlpatterns = [
    url(r'^admin/', admin.site.urls),
    # 支付宝返回url
    url(r'^alipa/', alipaview, name='alipa'),
]