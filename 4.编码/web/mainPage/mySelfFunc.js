function userName() {
    console.debug(document.cookie)
    s= document.cookie
    s = s.slice(6)
    console.debug(s)
    s= s+ "<img src=\"img/logo.png\" width=\"160\">"
    document.getElementById("welcome-user").innerHTML="欢迎:  "+s
}

function isVip() {
    $.ajax({
        type:"get",
        url: "https://www.zjyxxn.cn/api/isvip",
        dataType:'text',
        xhrFields: {
            withCredentials: true // 发送Ajax时，Request header中会带上 Cookie 信息。
        },
        crossDomain: true, // 发送Ajax时，Request header 中会包含跨域的额外信息，但不会含cookie（作用不明，不会影响请求头的携带）
        success:function (data) {
            alert(data)
            if(data == "0"){
                $.ajax({
                    type:"get",
                    url: "https://www.zjyxxn.cn/api/geturl",
                    dataType:'text',
                    xhrFields: {
                        withCredentials: true // 发送Ajax时，Request header中会带上 Cookie 信息。
                    },
                    crossDomain: true, // 发送Ajax时，Request header 中会包含跨域的额外信息，但不会含cookie（作用不明，不会影响请求头的携带）
                    success:function (payurl) {
                        console.debug(payurl)
                        window.location.href=payurl
                    },
                })
            }else{
                document.getElementById("advanPart").innerHTML="<a>高级功能部分</a><ul><li><a href=\"headDetect.html\">人数检测</a></li><li><a href=\"textDetect.html\">中文文本检测</a></li><li><a href=\"textDetectCSER.html\">文本检测(CSER)</a></li></ul>"
            }

        },
    })
}
